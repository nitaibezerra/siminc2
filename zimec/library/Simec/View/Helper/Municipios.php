<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Municipios extends Simec_View_Helper_Element
{
    public function municipios($name, $label = null, $value = null, $attribs = array(), $config = array())
    {


        $attribs = (array) $attribs;

        foreach($attribs as $chave => $attrib){
            if(is_numeric($chave)){
                $attribs[$attrib] = $attrib;
                unset($attribs[$chave]);
            }
        }


        $xhtml = '



                                <select
                                        multiple="multiple"
                                        size="5"
                                        name="listaMunicipio[]"
                                        id="listaMunicipio"
                                        ondblclick="abrirPopupListaMunicipio();"
                                        class="CampoEstilo link"
                                        style="width:400px;" >';

                                        if (!empty($_REQUEST['listaMunicipio'][0])) {

                                             function carregarMunicipioPorMuncod($muncod) {
                                                 global $db;
                                                $sql = "  SELECT
                                                                muncod AS codigo,
                                                                estuf ||' - '|| mundescricao AS descricao
                                                            FROM
                                                                territorios.municipio
                                                            WHERE muncod = '{$muncod}'
                                                            ORDER BY
                                                                mundescricao;";
                                                return $db->pegaLinha($sql);
                                            }

                                            foreach ($_REQUEST['listaMunicipio'] as $value) {
                                                $arrMunicipio = carregarMunicipioPorMuncod($value);
                                                $xhtml .= "<option value=\"{$arrMunicipio['codigo']}\">{$arrMunicipio['descricao']}</option>";
                                            }
                                        } else {
                                            $xhtml .= "<option value=\"\">Duplo clique para selecionar da lista</option>";
                                        }
                                    $xhtml .= '
                                        </select>
                                        <button type="button" class="btn btn-primary" data-toggle="modal" data-target="#modalMunicipios" id="BtmodalMunicipios" style="display: none">
                                              Filtrar
                                       </button>


                                        <div class="modal inmodal fade" id="modalMunicipios" tabindex="-1" role="dialog" aria-hidden="true">
                                <div class="modal-dialog">
                                    <div class="modal-content">
                                        <div class="modal-header">
                                            <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">×</span><span class="sr-only">Close</span></button>
                                            <h4 class="modal-title">Filtro de Municípios</h4>
                                        </div>
                                        <div class="modal-body">
                                                  <iframe src="http://'.$_SERVER["SERVER_NAME"].'/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario" name="municipios" width=100% height=400 scrollbars=1 frameborder=0"></iframe>
                                        </div>


                                    </div>
                                </div>
                            </div>



     ';?>
        <script>
            function abrirPopupListaMunicipio() {
                    $("#BtmodalMunicipios" ).trigger( "click" );
                    window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario', 'municipios', 'width=400,height=400,scrollbars=1');
            }
        </script>
<?php


        return $this->buildField($xhtml, $label, $attribs, $config);
    }
}
