<?php

/**
 * Constroi header padrao SASE
 *
 * Função que retorna cabecalho com arquivos necessários inclusos para
 * CSS e JS. Códigos inclusos copiados de cabecalho_boostrap.inc .
 */
function get_header(){
	global $estado;
	?>
	<!-- Bootstrap CSS -->
    <!-- <link href="../library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet" media="screen"> -->
    <!-- jQuery JS -->
    <script src="../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
    <!-- Bootstrap JS -->
    <!-- <script src="../library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script> -->

    <!-- simec -->
    <link href="/library/simec/css/custom.css" rel="stylesheet" media="screen">

    <div style="
        width:786px !important;
        height:615px !important;
        margin:0px;
        overflow:hidden;">

    <h3 class="topo_bootstrap_off" style="float:left;margin:10px;font-size:24px;"><?=$estado?>&nbsp;-&nbsp;</h3><div class="topo_bootstrap_off" id="map_canvastxt"></div>
    <div style="clear:both;height:1px;">&nbsp;</div>

	<style> 

		.titulo_sistema{ float:left;text-align:center;width:310px;font-weight: bolder;margin-top:11px; }

	    .rodape{
	    	position:fixed;
	    	bottom:0px;
	    }

	</style>
	<?php
}

function get_header2(){
	global $estado;
	?>
	<!-- jQuery JS -->
    <script src="../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>

    <style> 

		.titulo_sistema{ float:left;text-align:center;width:310px;font-weight: bolder;margin-top:11px; }

	    .rodape{
	    	position:fixed;
	    	bottom:0px;
	    }

	</style>
    <div>
	<?php
}

/**
 * Constroi footer padrao SASE
 *
 * Função que desenha rodapé default.
 */
function get_footer(){
	?>
    </div>
	<div class="row cont rodape" style="height: 45px; color: #667">
        <br />
            <div class="col-md-5">
                <ul>
                    <li>
                        Data: <?= date("d/m/Y - H:i:s") ?>
                    </li>
                </ul>
            </div>
            <div class="col-md-2 text-center">
                <ul>
                    <li>&nbsp;</li>
                </ul>
            </div>
            <div class="col-md-4 text-right">
                <address>
                    SIMEC - Fale Conosco Manual	| Tx.: 0,2015s / 0,34 
                </address>
            </div><br />
    </div>
	<?php
}


// COMPONENTES PARA MAPA ----------------------------------------------------------------
// TODO separar isso e talvez construir um objeto para isso

/**
 *
 */
function BuscaMunicipio( $estuf ){
    global $db;

    ob_clean(); 

    $municipios = $db->carregar( "select muncod, mundescricao from territorios.municipio where estuf = '".$estuf."'" ); ?>

    <div>
        <select name="muncod" id="muncod" onchange="Mapas.buscaMunicipio(this.value)">
            <option>Buscar Município:</option>
            <?php foreach ($municipios as $key => $value) { ?>
                <option value="<?=$value['muncod']?>"><?=$value['mundescricao']?></option>
            <?php } ?>
        </select>
    </div>

    <?php exit;
}

function getOptions(array $dados, array $htmlOptions = array(), $idCampo = null, $funcao = null, $value = null) {
	$html = '';
	$selected = '';
	$id = str_replace("]","",str_replace("[","",$idCampo));
	
	$html .= "<select class=\"form-control chosen\" id=\"{$id}\" name=\"{$idCampo}\"";
	if ($funcao != null){
		$html .= "onchange=\"{$funcao}\"";
	}
	$html .= ">";

	if (isset ( $htmlOptions ['prompt'] )) {
		$html .= '<option value="">' . strtr ( $htmlOptions ['prompt'], array (
				'<' => '&lt;',
				'>' => '&gt;'
		) ) . "</option>\n";
	}

	if ($dados) {
		foreach ( $dados as $data ) {
			/*if ($idCampo) {
				$selected = ($data ['codigo'] === trim($this->arAtributos[$idCampo]) ? "selected='true' " : "");
			}
			if ($value != null){
				$selected = ($data ['codigo'] === $value ? "selected='true' " : "");
			}*/
            $sel = '';
            if ($data['codigo'] == $value){
                $sel = "selected=\"true\"";
            }
			$html .= "<option {$selected}  title=\"{$data['descricao']}\" ".$sel." value= " . $data ['codigo'] . ">  " . simec_htmlentities( $data ['descricao'] ) . " </option> ";
		}
	}

	$html .= '</select>';

	return $html;
}


function CampoDif($sql, $sqlTab, $campos = array(), $titulo, $chave, $descricao, $descMaxLength = 500, $id, $campo = false, $funcaoDel = ''){
	global $db;
	
	if ($sql != '' && !empty($campos) && $chave != ''){

		$res = $db->carregar($sql);

        if ($campo){
		?>
		<script>
			function adicionarCampo<?= $id ?>(campo, TabBody){
				text = campo.get(0).options[campo.get(0).selectedIndex].text;
				val = campo.get(0).options[campo.get(0).selectedIndex].value;
                id = campo.attr('id');
                con = $('#countTr<?= id ?>').val();
                con++;
                if (val != '') {
                    if ($("#<?= $id.$chave ?>_" + val).length){
                        console.error('Município já selecionado.');
                    } else {
                        $('#' + TabBody).append("<tr id=\""+con+"\"><td><a href=\"javascript:apagaRegistro<?= id ?>('','"+con+"')\" title=\"Apagar\"><span class=\"glyphicon glyphicon-trash\"></span></a></td><td><input type=\"hidden\" name=\"<?= $id ?>codTab[]\" value=\"\"/><input type=\"hidden\" name=\"<?= $id.$chave ?>[]\" id=\"<?= $id.$chave ?>_" + val + "\" value=\"" + val + "\" />" + text + "</td><td><textarea class=\"form-control\" id=\"<?= $id.$descricao ?>_" + val + "\" maxlength=\"<?= $descMaxLength ?>\" name=\"<?= $id.$descricao ?>[]\" rows=\"2\" cols=\"5\" ></textarea></td></tr>");
                    }
                } else {
                    console.info('Valor do campo com id "'+id+'" está sem valor.');
                }
                $('#countTr<?= id ?>').val(con);
			}

            function apagaRegistro<?= id ?>(codigo, con){
                //alert(codigo);
                con = typeof con !== 'undefined' ? con : '';
                if (confirm("Deseja apagar este registro?")) {
                    if (codigo != '') {
                        <?php

                         if ($funcaoDel != ''){
                            echo $funcaoDel.'(codigo)';
                         } else {
                            echo "console.error('Funçao de deleção do registro não informada.');";
                         }

                        ?>
                    } else {
                        console.info('Código não possui valor. CON: '+con);
                        //con = $('#countTr<?= id ?>').val();
                        $('#'+con).remove();
                    }
                }
            }
		</script>
<!--	    <div class="form-group">-->
<!--	    	<label --><?//= $titulo == '' ? 'style="display: none;"' : '' ?><!-- for="dmdtipo" class="col-lg-2 col-md-2 control-label">--><?//= $titulo != '' ? $titulo.':' : '' ?><!--</label>-->
<!--	    	<div class="col-lg-4 col-md-4">-->
<!--		        <select name="--><?//= $id ?><!--comboD" id="--><?//= $id ?><!--comboD" class="form-control chosen">-->
<!--		            <option value="">Selecione...</option>-->
<?php
//
//                    if (is_array($res)){
//                        foreach ($res as $r) {
//                            echo <<<HTML
//                                    <option value="{$r['codigo']}">{$r['descricao']}</option>
//HTML;
//
//                        }
//                    }
?>
<!--		        </select>-->
<!--	        </div>-->
<!--	        <div --><?//= $titulo == '' ? 'class="col-lg-8 col-md-8"' : 'class="col-lg-6 col-md-6"' ?><!-->
<!--				<button title="Novo" class="btn btn-info" type="button" id="btnAdd" onclick="adicionarCampo><?= $id ?>($('#<?= $id ?>comboD'), '<?= $id ?>TabBody')" -->
<!--				</button>-->
<!--			</div>-->
<!--	    </div>-->
        <?php } ?>
	    <div id="listagem" style="background: #ffffff !important;">
            <input type="hidden" name="countTr<?= id ?>" id="countTr<?= id ?>" value="0"/>
			<table id="<?= $id ?>Tab" class="table table-bordered table-hover table-condensed" border="1">
				<thead>
					<tr>
                        <th width="33"></th>
						<th><?= $campos[0] ?></th>
						<th><?= $campos[1] ?></th>
					</tr>
				<thead>
				<tbody id="<?= $id ?>TabBody">
					<?php

                        if ($sqlTab != ''){
                            $res = $db->carregar($sqlTab);

                            if (is_array($res)) {
                                foreach ($res as $r) {
                                    ?>
                                    <tr>
                                        <td class="text-center"><a href="javascript:apagaRegistro<?= id ?>(<?= $r['codigo'] ?>)" title="Apagar"><span class="glyphicon glyphicon-trash"></span></a></td>
                                        <td>
                                            <input type="hidden" name="<?= $id . $chave ?>[]"
                                                   id="<?= $id . $chave . '_' . $r['chaveval'] ?>"
                                                   value="<?= $r['chaveval'] ?>"/>
                                            <input type="hidden" name="<?= $id ?>codTab[]"
                                                value="<?= $r['codigo'] ?>"/>
                                            <?= $r['mundescricao'] ?>
                                        </td>
                                        <td>
                                            <?php if ($campo) { ?>
                                            <textarea class="form-control" maxlength="<?= $descMaxLength ?>"
                                                      id="<?= $id . $descricao ?>_<?= $r['chaveVal'] ?>"
                                                      name="<?= $id . $descricao ?>[]" rows="2" cols="5"><?= $r[$descricao] ?></textarea>
                                            <?php } else { echo $r[$descricao]; } ?>
                                        </td>
                                    </tr>
                                <?php
                                }
                            } else {
                                ?>
                                <tr>
                                    <td colspan="3" style="text-align: center;">
                                        Sem registros
                                    </td>
                                </tr>
                                <?php
                            }
                        }

                    ?>
				</tbody>
	    	</table>
    	</div>
		<?php 		
	
	}
	
} 

// --