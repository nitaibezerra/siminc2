<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Boolean extends Simec_View_Helper_Element
{
    public function boolean($name, $label = null, $value = null, $attribs = array(), $config = array())
    {
        $attribs = (array) $attribs;

        if(!isset($attribs['checked']) && $value == 't'){
            $attribs['checked'] = 'checked';
        }

        foreach($attribs as $chave => $attrib){
            if(is_numeric($chave)){
                $attribs[$attrib] = $attrib;
                unset($attribs[$chave]);
            }
        }

        $id = isset($attribs['id']) ? $attribs['id'] : $name;
        $type = isset($attribs['type']) ? $attribs['type'] : 'checkbox';
        $class = isset($attribs['class']) ? 'form-control ' .  $attribs['class'] : ' js-switch ';
        $help = isset($attribs['help']) ? true : false;
        
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $id;
        $config['visible'] = isset($config['visible']) ? $config['visible'] : true;

        if ($help) {
        	$help = "<span class='help-block m-b-none'><i class='fa fa-question-circle' style='color: #1c84c6;'></i> {$attribs['help']}</span>";
        }
        
        unset($attribs['id'], $attribs['type'], $attribs['class'], $attribs['help'], $attribs['']);

        $podeEditar = isset($config['pode-editar']) ? $config['pode-editar'] : true;
        if(!$podeEditar || $podeEditar==='N'){
            $xhtml = '<p class="form-control-static" id="' . $id . '">' . $value . '</p>';
        } else {

//            <input type="checkbox" name="benempenhado" id="benempenhado" value="t" class="js-switch"  />

            // Construindo o input
            $xhtml = '<input'
                    . ' name="' . $this->view->escape($name) . '"'
                    . ' id="' . $this->view->escape($id) . '"'
                    . ' type="' . $type . '"'
                    . ' value="t"'
                    . ' class="' . $class . '"'
                    . $this->_htmlAttribs($attribs)
                    . ' />';

            $xhtml.= $help;
        }

        return $this->buildField($xhtml, $label, $attribs, $config);
    }
}
