<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_FormInput extends Simec_View_Helper_FormElement
{
    public function formInput($name, $label = null, $value = null, $attribs = null, $config = array())
    {
        $id = isset($attribs['id']) ? $attribs['id'] : $name;
        $type = isset($attribs['type']) ? $attribs['type'] : 'text';
        $required = is_array($attribs) && in_array('required', $attribs) ? 'required="required"' : '';
        $disabled = is_array($attribs) && in_array('disabled', $attribs) ? 'disabled="disabled"' : '';
        $class = isset($attribs['class']) ? 'form-control ' .  $attribs['class'] : 'form-control';
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $id;

        if($value && strpos($class, 'data') !== false){
            $value = Simec_Util::formatarData($value, Zend_Date::DATES);
        }

        unset($attribs['id'], $attribs['type'], $attribs['class'], $attribs['required'], $attribs['disabled']);

        // Construindo o input
        $xhtml = '<input'
                . ' name="' . $this->view->escape($name) . '"'
                . ' id="' . $this->view->escape($id) . '"'
                . ' type="' . $type . '"'
                . ' value="' . $value . '"'
                . ' class="' . $class . '"'
                . ' ' . $required . ' '
                . ' ' . $disabled . ' '
                . $this->_htmlAttribs($attribs)
                . " />";

        return $this->buildField($xhtml, $label, $config);
    }
}
