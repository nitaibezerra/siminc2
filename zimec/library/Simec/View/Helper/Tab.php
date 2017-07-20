<?php
/**
 * Abstract class for extension
 */

class Simec_View_Helper_Tab
{
    public function tab($itens = array(), $url = false, $config = array())
    {
        $url = $url ? $url : $_SERVER['REQUEST_URI'];
        
        $xhtml = '<div class="tabs-container">';
	    $xhtml .= '    <ul class="nav nav-tabs">';
	    
	    foreach ($itens as $tab) {
	    	if (is_array($tab)) {
	    		$active = (strpos($url, $tab['link']) !== false) ? 'class="active"' : '';
	    		$expanded = ($tab['link'] == $url)? true : false;
	    		$xhtml .= '<li ' . $active . '><a href="' . $tab['link'] . '">' . $tab['descricao'] . '</a></li>';
	    	}
	    }
	    
	    $xhtml .= '    </ul>';
	    $xhtml .= '</ul>';
	    
	    return $xhtml;
    }
}
