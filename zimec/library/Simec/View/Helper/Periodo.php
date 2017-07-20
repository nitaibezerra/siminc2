<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Periodo extends Simec_View_Helper_Element
{
    public function periodo($name, $label = null, $value = null, $attribs = array(), $config = array())
    {
        $name = (array) $name;
        $value = (array) $value;
        $attribs = (array) $attribs;

        foreach($attribs as $chave => $attrib){
            if(is_numeric($chave)){
                $attribs[$attrib] = $attrib;
                unset($attribs[$chave]);
            }
        }

        $nameInicio = $idInicio = count($name) == 1 ? $name[0] . 'inicio' : $name[0];
        $nameFim = $idFim = count($name) == 1 ? $name[0] . 'fim' : $name[1];

        $type = isset($attribs['type']) ? $attribs['type'] : 'text';
        $class = isset($attribs['class']) ? 'form-control ' .  $attribs['class'] : 'form-control';

        $config['date'] = 'input-daterange';
//        $config['icon'] = $config['icon'] ? $config['icon'] : 'fa fa-calendar';
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $idInicio;

        unset($attribs['id'], $attribs['type'], $attribs['class']);

        $valueInicio = !empty($value[0]) ? Simec_Util::formatarData($value[0], Zend_Date::DATES) : null;
        $valueFim = !empty($value[1]) ? Simec_Util::formatarData($value[1], Zend_Date::DATES) : null;

        $podeEditar = isset($config['pode-editar']) ? $config['pode-editar'] : true;
        if(!$podeEditar || $podeEditar==='N'){
            $inicio = $valueInicio ? 'De ' . $valueInicio : '';
            $fim = $valueFim ? ' até ' . $valueFim : '';
            $xhtml = '<p class="form-control-static" id="' . $nameInicio . '">' . $inicio . $fim . '</p>';
            $config['icon'] = $config['date'] = null;
        } else {

            $xhtml = '<div class="input-group">
                        <span class="input-group-addon" style="padding: 4px 12px !important; background-color: #fff;"><span class="fa fa-calendar"></span></span></span>';

            $xhtml .= '<input'
                . ' name="' . $this->view->escape($nameInicio) . '"'
                . ' id="' . $this->view->escape($idInicio) . '"'
                . ' type="' . $type . '"'
                . ' value="' . $valueInicio . '"'
                . ' class=" ' . $class . '"'
                . $this->_htmlAttribs($attribs)
                . ' />';

            $xhtml .= '<span class="input-group-addon" style="padding: 4px 12px !important; background-color: #fff;">a</span>';

            $xhtml .= '<input'
                . ' name="' . $this->view->escape($nameFim) . '"'
                . ' id="' . $this->view->escape($idFim) . '"'
                . ' type="' . $type . '"'
                . ' value="' . $valueFim . '"'
                . ' class=" ' . $class . '"'
                . $this->_htmlAttribs($attribs)
                . ' />';

            $xhtml .= '</div>';
        }

        return $this->buildField($xhtml, $label, $attribs, $config);
    }
}
