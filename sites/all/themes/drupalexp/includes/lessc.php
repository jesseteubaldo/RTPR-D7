<?php

require_once dirname(__FILE__) . '/lessc.inc.php';

class drupalexp_lessc {

    var $theme;
    var $output;
    var $css;
    var $lessc;
    var $importDir;

    function __construct($theme) {
        $this->theme = $theme;
        $this->lessc = new lessc();
        $this->lessc->setImportDir(drupal_get_path('theme', $theme->theme));
        $this->importDir = drupal_get_path('theme', $theme->theme);
        $this->__addPresetVariables();
        $this->__addBaseCSS();
    }

    function addVariable($name, $value) {
        $this->output .= "@{$name}:{$value};\n";
    }

    function filetime($file) {
        if (($time = @filemtime($file)) != FALSE) {
            return $time;
        }
        if (($time = @filemtime($this->importDir . '/' . $file)) != FALSE) {
            return $time;
        }
        return 0;
    }

    function complie($file = null) {
        $update = false;
        $theme_path = drupal_get_path('theme', $this->theme->theme);
        //print $theme_path.'<br>'.DRUPAL_ROOT.'<br/>';
        //print file_create_url($file).'<br/>';
        $assets_path = file_create_url($theme_path.'/assets');
        //$theme_url = url('<front>',array('absolute'=>true)).str_replace($GLOBALS['base_url'],'',$theme_path);
        //print url('<front>',array('absolute'=>true)).'<br/>';
        //print url('<front>',array('absolute'=>false)).'<br/>';
        //print $theme_path;die;
        //$assets_path = $theme_url . '/assets';
        //$assets_path = str_replace('//','/',$assets_path);
        $drupalexp_assets = variable_get('drupalexp_assets_path','');
        if($drupalexp_assets != $assets_path){
            $update = true;
            variable_set('drupalexp_assets_path',$assets_path);
        }
        $ftime = $this->filetime($file);
        if (!empty($this->theme->lessc)) {
            foreach ($this->theme->lessc as $lessc_file) {
                if ($ftime < $this->filetime($lessc_file)) {
                    $update = true;
                }
                $this->output .= "@import \"$lessc_file\";\n";
            }
        }
        if ($update) {
            try {
                $this->css = $this->lessc->compile($this->output);
            } catch (exception $e) {
                drupal_set_message("fatal error: " . $e->getMessage(), 'error');
                return FALSE;
            }
            if ($file) {
                $css_output = "/*This file is generated by less css (http://lesscss.org) using drupalexp framework (http://drupalexp.com)*/\n/*Please do not modify this file content*/\n" . $this->css;
                $css_output = str_replace(array('../'),$assets_path.'/',$css_output);
                file_unmanaged_save_data($css_output, $file, FILE_EXISTS_REPLACE);
            }
        }
        return $this->css;
    }

    private function __addPresetVariables() {
        foreach ($this->theme->lessc_vars as $key => $value) {
            $this->addVariable($key, $value);
        }
    }

    private function __addBaseCSS() {
        $this->output .= 'body{color: @text_color;}a:not(.btn){color:@link_color; &:hover{color:@link_hover_color}}h1,h2,h3,h4,h5,h6{color:@heading_color}';
    }

}
