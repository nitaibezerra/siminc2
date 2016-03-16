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
	    		$active = ($tab['link'] == $url)? 'class="active"' : '';
	    		$expanded = ($tab['link'] == $url)? true : false;
	    		$xhtml .= '<li ' . $active . '><a href="' . $tab['link'] . '">' . $tab['descricao'] . '</a></li>';
	    	}
	    }
	    
	    $xhtml .= '    </ul>';
	    $xhtml .= '</ul>';
	    
	    return $xhtml;
	    /*
        </div>
	        <div id="tab-2" class="tab-pane">
		        <div class="panel-body">
			        <strong>Donec quam felis</strong>
			        
			        <p>Thousand unknown plants are noticed by me: when I hear the buzz of the little world among the stalks, and grow familiar with the countless indescribable forms of the insects
			        and flies, then I feel the presence of the Almighty, who formed us in his own image, and the breath </p>
			        
			        <p>I am alone, and feel the charm of existence in this spot, which was created for the bliss of souls like mine. I am so happy, my dear friend, so absorbed in the exquisite
			        sense of mere tranquil existence, that I neglect my talents. I should be incapable of drawing a single stroke at the present moment; and yet.</p>
		        </div>
	        </div>
        </div>
        
        
        </div>
        
        $xhtml .= '<div class="row notprint">';
        $xhtml .= '	 <div class="col-md-12">';
        $xhtml .= '		<ul class="nav nav-tabs">';
        
       
        
        $xhtml .= '		</ul>';
        $xhtml .= '	  </div>';
        $xhtml .= '</div>';
        
        return $menu;
        
        // Construindo o input
        $xhtml = '<textarea'
                . ' name="' . $this->view->escape($name) . '"'
                . ' id="' . $this->view->escape($id) . '"'
                . ' type="' . $type . '"'
                . ' class="' . $class . '"'
                . ' ' . $required . ' '
                . ' ' . $disabled . ' '
                . $this->_htmlAttribs($attribs)
                . ">$value</textarea>";

        return $this->buildField($xhtml, $label, $attribs);
        */
    }
}
