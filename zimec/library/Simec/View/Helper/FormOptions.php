<?php
/** 
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_FormOptions extends Simec_View_Helper_FormElement
{
    public function formOptions($name, $label = null, $value = null, $options = null, $attribs = null, $config = array())
    {
        $id = isset($attribs['id']) ? $attribs['id'] : $name;
        $type = isset($attribs['type']) ? $attribs['type'] : 'radio';
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $id;
        $style = isset($config['style']) ? $config['style'] : 'button';

        switch ($style) {
            case ('inline'):
                $xhtml = '<div>';
                foreach ($options as $valor => $descricao) {
                    $xhtml .= ' <label class="' . $type . '-inline">
                                    ' . $this->montarInput($name, $valor, $descricao, $attribs, $config) . '
                                </label>';
                }
                $xhtml .= '</div>';
                break;
            case ('lista'):
                $xhtml = '<div>';
                foreach ($options as $valor => $descricao) {
                    $xhtml .= ' <div class="' . $type . '">
                                    <label>
                                        ' . $this->montarInput($name, $valor, $descricao, $attribs, $config) . '
                                    </label>
                                </div>';
                }
                $xhtml .= '</div>';
                break;
            default:
                $xhtml = '<div class="btn-group" data-toggle="buttons">';
                foreach ($options as $valor => $descricao) {
                    $xhtml .= ' <label class="btn btn-primary">
                                    ' . $this->montarInput($name, $valor, $descricao, $attribs, $config) . '
                                </label>';
                }
                $xhtml .= '</div>';
            break;
        }

        return $this->_build($xhtml, $label, $config);
    }

    protected function montarInput($name, $valor, $descricao, $attribs = null, $config = null)
    {
        $id = isset($attribs['id']) ? $attribs['id'] : $name;
        $type = isset($attribs['type']) ? $attribs['type'] : 'radio';
        $required = is_array($config) && in_array('required', $config) ? 'required="required"' : '';
        $class = isset($attribs['class']) ? 'form-control ' .  $attribs['class'] : '';

        unset($attribs['id'], $attribs['type'], $attribs['class']);

        // Construindo o input
        return    '<input'
                . ' name="' . $this->view->escape($name) . '"'
                . ' id="' . $this->view->escape($id) . '"'
                . ' type="' . $type . '"'
                . ' value="' . $valor . '"'
                . ' class="' . $class . '"'
                . ' ' . $required . ' '
                . $this->_htmlAttribs($attribs)
                . " />" . $descricao;
    }

    protected function _build($xhtml, $label, $config)
    {
        $icon = !empty($config['icon']) ? '<span class="input-group-addon"><span class="glyphicon glyphicon-' . $config['icon'] . '"></span></span></span>' : null;
        $help = !empty($config['help']) ? '<span class="input-group-addon help-tooltip" data-toggle="tooltip" data-placement="left" title="' . $config['help'] . '"><span class="glyphicon glyphicon-question-sign"></span></span>' : null;

        if ($icon || $help) {
            $xhtml = '
                <div class="input-group">
                    ' . $icon . '
                    ' . $xhtml . '
                    ' . $help . '
                </div>
            ';
        }

        if ($label) {
            $required = is_array($config) && in_array('required', $config) ? '<span class="campo-obrigatorio" title="Campo obrigatório">*</span>' : '';
            $xhtml =  '
                <div class="form-group">
                    <label for="intcnpj" class="col-lg-2 control-label">' . $label . ': ' . $required . '</label>
                    <div class="col-lg-10">
                        '. $xhtml .'
                    </div>
                </div>
            ';
        }

//ver(htmlentities($xhtml), d);
        return $xhtml;
    }
}
