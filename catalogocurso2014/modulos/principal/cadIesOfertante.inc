<?php
global $db;
$curid = $_SESSION['catalogo']['curid'];

if(!$curid){
    echo "<script>
			window.location = 'catalogocurso2014.php?modulo=inicio&acao=C';
		  </script>";
    exit;
}

if(isset($_POST['action']) && !empty($_POST['action'])){
    $action = $_POST['action'];
} else {
    $action = '';
}
    switch($action):
        case 'salvar':
            if(isset($_POST['ies']) && !empty($_POST['ies'])){
                $arrIeoId = $_POST['ieoid'];
                $arrIdIES = $_POST['ies'];
                $arrUO = $_POST['uo'];
                $arrAno = $_POST['ano'];
                $arrQtdVagas = $_POST['qtd_vagas'];
                $arrNomeIES = $_POST['nome_ies'];
                $arrValorEstimado = $_POST['valor_estimado'];

                $sql = '';
                $arrId = array();
                $arrLogErro = array();
                foreach($arrIdIES as $key => $idIES){
                    $nomeIES = utf8_decode($arrNomeIES[$key]);

                    $isValid = true;
                    // Validando Ano.
                    if($_SESSION['exercicio']==ANO_EXERCICIO_2014){
	                    if(empty($arrAno[$key])){
	                        $isValid = false;
	                        $arrLogErro[] = "<strong>{$nomeIES}</strong> - Ano não pode estar vazio!";
	                    } else if($arrAno[$key] < 2013 || $arrAno[$key] > 2500){
	                        $isValid = false;
	                        $arrLogErro[] = "<strong>{$nomeIES}</strong> - Ano inválido! ( > 2012 )";
	                    }
                    }

                    // Validando quantidade de vagas.
                    if(empty($arrQtdVagas[$key])){
                        $isValid = false;
                        $arrLogErro[] = "<strong>{$nomeIES}</strong> - Quantidade de vagas não pode estar vazia!";
                    } else if($arrQtdVagas[$key] < 1){
                        $isValid = false;
                        $arrLogErro[] = "<strong>{$nomeIES}</strong> - Quantidade de vagas é inválida!";
                    }

                    // Validando valor estimado.
                    if(empty($arrValorEstimado[$key])){
                        $isValid = false;
                        $arrLogErro[] = "<strong>{$nomeIES}</strong> - Valor estimado, não pode estar vazia!";
                    } else if($arrQtdVagas[$key] < 1){
                        $isValid = false;
                        $arrLogErro[] = "<strong>{$nomeIES}</strong> - Valor estimado é inválido!";
                    } else {
                        $arrValorEstimado[$key] = str_replace(array('.' , ',') , array('' , '.') , $arrValorEstimado[$key]);
                    }
                    
                    if($_SESSION['exercicio']==ANO_EXERCICIO_2014){
                    	$ieoanoprojeto = $arrAno[$key];
                    } else {
                    	$ieoanoprojeto = $_SESSION['exercicio'];
                    }

                    if($isValid){
                        if(isset($arrIeoId[$key]) && !empty($arrIeoId[$key])){
                            $sql = "UPDATE 		catalogocurso2014.iesofertante
                                    SET 		curid = {$curid}, 
                                    			unicod = {$arrUO[$key]}, 
                                    			ieoanoprojeto = {$ieoanoprojeto}, 
                                    			ieoqtdvagas = {$arrQtdVagas[$key]}, 
                                    			ieostatus = 'A', 
                                    			entid={$idIES}, 
                                    			valor_estimado = {$arrValorEstimado[$key]}
                                     WHERE 		ieoid = {$arrIeoId[$key]}
                                     RETURNING 	ieoid;";
                        } else {
                            $sql = "INSERT INTO catalogocurso2014.iesofertante(curid, unicod, ieoanoprojeto, ieoqtdvagas, ieostatus, entid, valor_estimado)
                                    VALUES ( {$curid}, {$arrUO[$key]}, {$ieoanoprojeto}, {$arrQtdVagas[$key]}, 'A', {$idIES}, {$arrValorEstimado[$key]}) RETURNING ieoid;";
                        }

                        $arrId[] = $db->pegaUm($sql);
                        $db->commit();
                    } else {
                        $arrId[] = $arrIeoId[$key];
                    }
                }


                if(isset($arrId) && !empty($arrId) && is_array($arrId) && count($arrId) > 0 && !empty($arrId[0]) ){

                    $arrId = array_filter($arrId);

                    $sql = "SELECT 	* 
                    		FROM 	catalogocurso2014.iesofertante
                            WHERE 	ieostatus != 'I' AND curid = {$curid} AND ieoid NOT IN (". implode(', ' , array_filter(array_values($arrId))) .")";
                    $temIES = $db->pegaLinha($sql);

                    if($temIES){
                        if(!empty($arrId[0])){
                            $sql = "UPDATE 	catalogocurso2014.iesofertante
                                    SET 	ieostatus = 'I'
                                    WHERE 	ieostatus != 'I' AND curid = {$curid} AND ieoid NOT IN (". implode(', ' , array_filter(array_values($arrId))) .")";
                            $db->executar($sql);
                            $db->commit();
                        } else {
                            $sql = "UPDATE 	catalogocurso2014.iesofertante
                                    SET 	ieostatus = 'I'
                                    WHERE 	ieostatus != 'I' AND curid = {$curid}";
                            $db->executar($sql);
                            $db->commit();
                        }
                    }

                    echo 'Dados salvo com sucesso!';

                    if(count($arrLogErro) > 0){
                        echo ' Porém algumas IES não foram salvas!';
                        $textoLog = str_replace(array('<strong>' , '</strong>') , '' ,$arrLogErro[0]);
                        echo ' - ' . $textoLog;
                    }
                } else {
                    echo 'Dados não foram salvos!';
                    $textoLog = str_replace(array('<strong>' , '</strong>') , '' ,$arrLogErro[0]);
                    echo ' - ' . $textoLog;
                }
            } else {
                $sql = "SELECT 	* 
                		FROM 	catalogocurso2014.iesofertante
                        WHERE 	ieostatus != 'I' AND curid = {$curid}";
                $temIES = $db->pegaLinha($sql);

                if($temIES){
                    $sql = "UPDATE 	catalogocurso2014.iesofertante
                            SET 	ieostatus = 'I'
                            WHERE 	ieostatus != 'I' AND curid = {$curid}";
                    $db->executar($sql);
                    $db->commit();
                    echo 'Dados salvo com sucesso!';
                }
            }

        	exit();
        	break;
        case 'cad_ies':

            $id = $_POST['ies'];
			
            if($_SESSION['exercicio']==ANO_EXERCICIO_2014){
	            $sql = "SELECT 		DISTINCT e.entid AS ies, 
	            					entunicod AS uo, 
	            					entnome AS descricao, 
	            					entsig
	                    FROM 		entidade.entidade e
	                    INNER JOIN 	entidade.funcaoentidade f ON f.entid = e.entid
	                    WHERE 		entstatus = 'A' AND e.entid = {$id}
	                    ORDER BY 	descricao";
            } else {
	            $sql = "SELECT 		DISTINCT entid AS ies,
	            					unicod AS uo, 
	            					ieaies AS descricao, 
	            					ieasigla
	                    FROM 		catalogocurso2014.iesofertanteapoio
	                    WHERE 		ieastatus = 'A' AND entid = {$id}
	                    ORDER BY 	descricao";            	
            }

            $ies = $db->pegaLinha($sql); ?>
            
            <?php $idRand = (double)microtime()*1000000;?>
            <tr class="linha_listagem" id="ies_<?php echo $idRand ?>" style="background-color: #fafafa;">
                <input type="hidden" name="ies[]" value="<?php echo $_POST['ies']; ?>">
                <input type="hidden" name="uo[]" value="<?php echo $ies['uo']; ?>">
                <input type="hidden" name="nome_ies[]" value="<?php echo $ies['descricao']; ?>">
                <!-- Ação -->
                <td nowrap="" style="text-align:center; width: 80px;">
                    <img onclick="javascript:excluirIES( '<?php echo $idRand ?>' )" style="cursor:pointer;" align="absmiddle" title="Excluir Ação" style="border: 0;" src="../imagens/excluir.gif">
                </td>
                <!-- Codigo UO -->
                <td><?php echo $ies['uo']; ?></td>
                <!-- Abreviação IES -->
                <td><?php echo $ies['entsig']; ?></td>
                <!-- Nome IES -->
                <td><?php echo $ies['descricao']; ?></td>
                <!-- Ano -->
                <?php if($_SESSION['exercicio']==ANO_EXERCICIO_2014){ ?>
                <td>
                    <?php
                    $ano = array();
                    $ano[] = array('codigo' => '2013' , 'descricao' => '2013');
                    $ano[] = array('codigo' => '2014' , 'descricao' => '2014');
                    echo $db->monta_combo('ano[]', $ano , 'S' ,$titulo='Selecione..',$acao,$opc,$txtdica='',$size='',$obrig='N', $id = '', $return = false, $value = null, $title= null, $complemento = null); ?>
                </td>
                <?php } ?>
                <!-- Qtd Vagas -->
                <td><?php echo campo_texto("qtd_vagas[]", 'N', 'S', '', '10', '6', '######', '', '' , '' , '' , '' , '' , ''); ?></td>
                <!-- Valor estimado -->
                <td><?php echo campo_texto("valor_estimado[]", 'N', 'S', '', '11', '14', '###.###.###,##', '', '' , '' , '' , 'class="soma"' , '' , ''); ?></td>
            </tr>
            <script language="javascript">
                $('input[name="valor_estimado[]"]').focusout(function(){contarTotal()});
                $('input[name="qtd_vagas[]"]').focusout(function(){contarTotal()});
            </script>
			<?php
            break;
        default:
            //Chamada de programa
            include  APPRAIZ."includes/cabecalho.inc";

            if($_SESSION['catalogo']['curid']){
                monta_abas();
            } else {
                monta_abas(57394);
            }
			
            if($_SESSION['exercicio']==ANO_EXERCICIO_2014){
	            $sql = "SELECT 		DISTINCT e.entid AS codigo,
	                    			e.entunicod,
	                    			entnome AS descricao
	                	FROM 		entidade.entidade e
	                    INNER JOIN 	entidade.funcaoentidade f ON f.entid = e.entid AND funid IN (12,11,44)
	                	WHERE       e.entunicod IS NOT NULL AND entstatus = 'A' AND entnome IS NOT NULL 
	                	ORDER BY    descricao";
            } else {
	            $sql = "SELECT 		DISTINCT entid AS codigo,
	            					unicod, 
	            					ieaies AS descricao
	                    FROM 		catalogocurso2014.iesofertanteapoio
	                    WHERE 		ieastatus = 'A' AND unicod IS NOT NULL
	                    ORDER BY 	descricao";    
            }

            $listaies = $db->carregar($sql);

            // IES Ofertante.
            // Recuperar dados IES Ofertante.
            
            if($_SESSION['exercicio']==ANO_EXERCICIO_2014){
	            $sqlIESOfertante = "SELECT 		DISTINCT valor_estimado,  
							                    ieo.ieoid, 
							                    ies.entid, 
							                    ieo.unicod, 
							                    ies.entsig, 
							                    ies.entnome, 
							                    ieo.curid, 
							                    ieo.ieoanoprojeto, 
							                    ieo.ieoqtdvagas 
	                				FROM 		catalogocurso2014.iesofertante ieo
	                    			LEFT JOIN 	entidade.entidade ies ON (ieo.entid = ies.entid)
	                    			LEFT JOIN 	entidade.funcaoentidade f ON f.entid = ies.entid AND funid IN (12,11,44)
	                				WHERE		entstatus = 'A' AND ieostatus = 'A' AND ieo.curid = {$curid}
	                				ORDER BY    ies.entsig, ies.entnome";
            } else {
	            $sqlIESOfertante = "SELECT 		DISTINCT valor_estimado,  
							                    ieo.ieoid, 
							                    ies.entid, 
							                    ieo.unicod, 
							                    ies.ieasigla, 
							                    ies.ieaies, 
							                    ieo.curid, 
							                    ieo.ieoanoprojeto, 
							                    ieo.ieoqtdvagas 
	                				FROM 		catalogocurso2014.iesofertante ieo
	                    			LEFT JOIN 	catalogocurso2014.iesofertanteapoio ies ON (ieo.entid = ies.entid)
	                				WHERE		ies.ieastatus = 'A' AND ieo.ieostatus = 'A' AND ieo.curid = {$curid}
	                				ORDER BY    ies.ieasigla, ies.ieaies";
            }

            $iesOfertante = $db->carregar($sqlIESOfertante);
            $ies = $db->pegaLinha($sql);

            if($_SESSION['catalogo']['curid'] ){
                $permissoes = testaPermissao();
            } else {
                $permissoes['gravar'] = true;
            } ?>
                <div class="row">
                    <div class="col-md-12">
                        <ul class="breadcrumb">
                            <li>
                                <strong>Curso: </strong>
                                <?php echo $_SESSION['catalogo']['curid'] .' - ' . $_SESSION['catalogo']['curdesc']; ?>
                            </li>
                        </ul>
                    </div>
                </div>
                <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
                    <tr>
                        <td class="SubTituloDireita" width="15%">IES</td>
                        <td>
                            <select name="ies[]" class="form-control chosen-select" required="required" data-placeholder="Selecione..." onchange="javascript:cadIES(this);" style="width:450px;">
                                <option value=""></option>
                                <?php foreach($listaies as $ie){ ?>
                                    <option value="<?php echo $ie['codigo']; ?>"><?php echo $ie['descricao']; ?></option>
                                <?php } ?>
                            </select>
                        </td>
                    </tr>
                </table>
            <script>
                var jq = jQuery.noConflict();
                jq('.chosen-select').chosen({allow_single_deselect:true});
            </script>
                <script src="/includes/JQuery/jquery-1.9.1/jquery-1.9.1.js" type="text/javascript"></script>
                <script src="/includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.min.js" type="text/javascript"></script>
                <script src="/includes/JQuery/jquery-1.9.1/funcoes.js" type="text/javascript"></script>
                <link href="/includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.min.css" rel="stylesheet">
            <form id="form_ies" name="form_ies">
                <table class="tabela listagem" align="center" border="0" cellpadding="5" cellspacing="1">
	                <thead>
                        <tr>
                            <td>Ação</td>
                            <td>Código UO</td>
                            <td>Sigla IES</td>
                            <td>Nome IES</td>
                            <?php if($_SESSION['exercicio']==ANO_EXERCICIO_2014){ ?>
                            <td>Ano</td>
                            <?php } ?>
                            <td>Qtd. Vagas</td>
                            <td>Valor estimado MEC (Custeio, em R$)</td>
                        </tr>
                    </thead>
                    <tbody id="container_list_ies">
                    	<?php if($iesOfertante){ ?>
                    		<?php foreach($iesOfertante as $ieOfertante){ ?>
                            <?php $idRand = (double)microtime()*1000000;?>
                            <tr class="linha_listagem" id="ies_<?php echo $idRand ?>" style="background-color: #fafafa;">
                                <input type="hidden" name="ieoid[]" value="<?php echo $ieOfertante['ieoid']; ?>">
                                <input type="hidden" name="ies[]" value="<?php echo $ieOfertante['entid']; ?>">
                                <input type="hidden" name="uo[]" value="<?php echo $ieOfertante['unicod']; ?>">
                                <input type="hidden" name="nome_ies[]" value="<?php echo $ieOfertante['entnome']; ?>">
                                <!-- Ação -->
                                <td nowrap="" style="text-align:center; width: 80px;">
                                   <img onclick="javascript:excluirIES( '<?php echo $idRand; ?>' )" style="cursor:pointer;" align="absmiddle" title="Excluir Ação" style="border: 0;" src="../imagens/excluir.gif">
                                </td>
                                <!-- Codigo UO -->
                                <td><?php echo $ieOfertante['unicod']; ?></td>
                                <!-- Abreviação IES -->
                                <td><?php echo $ieOfertante['entsig']; ?></td>
                                <!-- Nome IES -->
                                <td><?php echo $ieOfertante['entnome']; ?></td>
                                <!-- Ano -->
                                <?php if($_SESSION['exercicio']==ANO_EXERCICIO_2014){ ?>
                                <td>
                                    <?php
                                    $ano = array();
                                    $ano[] = array('codigo' => '2013' , 'descricao' => '2013');
                                    $ano[] = array('codigo' => '2014' , 'descricao' => '2014');
                                    echo $db->monta_combo('ano[]', $ano , 'S' ,$titulo='Selecione..',$acao,$opc,$txtdica='',$size='',$obrig='N', $id = '', $return = false, $value = $ieOfertante['ieoanoprojeto'], $title= null, $complemento = null); ?>
                                </td>
                                <?php } ?>
                                <!-- Qtd Vagas -->
                                <td>
                                    <?php echo campo_texto("qtd_vagas[]", 'N', 'S', '', '10', '6', '######', '', '' , '' , '' , '' , '' , $ieOfertante['ieoqtdvagas']); ?>
                                </td>
                                <td>
                                    <?php echo campo_texto("valor_estimado[]", 'N', 'S', '', '11', '14', '###.###.###,##', '', '' , '' , '' , 'class="soma"' , '' , number_format( $ieOfertante['valor_estimado'], 2, ',', '.')); ?>
                                </td>
                            </tr>
                        	<?php } ?>
                        <?php } ?>
					</tbody>
                    <tbody>
                        <tr style="background-color: #E3E3E3; font-weight: bold;">
                            <!-- Ação -->
                            <td nowrap="" style="text-align:center; width: 80px; font-weight: bold;">Total</td>
                            <!-- Codigo UO -->
                            <td colspan="<?php echo ($_SESSION['exercicio']==ANO_EXERCICIO_2014) ? '4' : '3'; ?>"></td>
                            <!-- Abreviação IES -->
                            <!-- Nome IES -->
                            <!-- Ano -->
                            <!-- Qtd Vagas -->
                            <td id="container_total_qtd_vagas" style="font-weight: bold;"></td>
                            <td id="container_total_valor_estimado" style="font-weight: bold;"></td>
                        </tr>
                	</tbody>
                </table>
			</form>
            
            <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
            	<tr>
                	<td colspan="2" style="text-align: center;">
                    	<input type="button" id="bt_voltar"  value="Voltar"/>
                    	<input type="button" id="bt_salvar" value="Salvar" />
                        <input type="button" id="bt_salvarC" value="Salvar e Continuar"/>
                        <input type="button" id="bt_proximo" value="Próximo"/>
                    </td>
                </tr>
            </table>
            <div id="dialog"></div>
                <script type="text/javascript">
                    $('#bt_voltar').click(function(){
                        window.location.href = 'catalogocurso2014.php?modulo=principal/cadPublicoAlvo&acao=A';
                    });

                    $('#bt_proximo').click(function(){
                        window.location.href = 'catalogocurso2014.php?modulo=principal/cadContato&acao=A';
                    });

                    $('#bt_salvar').click(function(){
                        salvar();
                    });

                    $('#bt_salvarC').click(function(){
                        salvar();
                        setTimeout(function(){
                            window.location.href = 'catalogocurso2014.php?modulo=principal/cadContato&acao=A';
                        } , 1500);
                    });


                    function salvar(){
                        data = jQuery('#form_ies').serialize() + '&action=salvar';
                        jQuery.post(window.location.href, data, function(html){
                            if(html != ''){
                                alert(html);
                            }
                        });
                    }

                    function cadIES(ies){
                        ies = jQuery(ies);
                        if(jQuery(ies).val()){
                            jQuery.post(window.location.href, {action:'cad_ies', ies: ies.val()}, function(html){
                                jQuery('#container_list_ies').hide().append(html).fadeIn();
                            });

                            jQuery(ies).val('');

                            setTimeout(function(){
                                jQuery('a.chosen-single.chosen-single-with-deselect').addClass('chosen-default')
                                jQuery('.chosen-single-with-deselect').addClass('chosen-default');
                                jQuery('.chosen-single-with-deselect').children('abbr').remove()
                                jQuery('.chosen-single-with-deselect').children('span').hide().html('Selecione..').fadeIn();
                            } , 1000);
                        }

                        <?php if(!$permissoes['gravar']){ ?>
                        	setTimeout(function(){
                            $('select , textarea , input').attr('disabled' , 'disabled');
                            $('input:button').removeAttr('disabled' , 'disabled');
                            $('#bt_salvar , #bt_salvarC').attr('disabled' , 'disabled');

                        } , 500);
                        <?php } ?>
                    }

                    function excluirIES(id){
                        jQuery('#ies_'+id).remove();
                        contarTotal();
                    }
                    <?php if(!$permissoes['gravar']){ ?>
                    setTimeout(function(){
                        $('select , textarea , input').attr('disabled' , 'disabled');
                        $('input:button').removeAttr('disabled' , 'disabled');
                        $('#bt_salvar , #bt_salvarC').attr('disabled' , 'disabled');

                    } , 500);
                    <?php } ?>

                    contarTotal();

                    $('input[name="valor_estimado[]"]').focusout(function(){contarTotal()});
                    $('input[name="qtd_vagas[]"]').focusout(function(){contarTotal()});

                    function contarTotal(){
                        var qtdVagasTotal = 0;
                        $('input[name="qtd_vagas[]"]').each(function(){
                            if($(this).val()){
                                qtdVagasTotal = qtdVagasTotal + parseInt($(this).val());
                            }
                        });

                        var soma = somarCampos('soma');
                        jQuery('#container_total_valor_estimado').html('R$ ' + number_format(soma, 2, ',', '.'));

                        if(qtdVagasTotal > 1) plural = 's'
                        else plural = '';

                        $('#container_total_qtd_vagas').hide().html(qtdVagasTotal + ' vaga' + plural).fadeIn();
                    }

                	$(document).ready(function() {
                		var exercicio = "<h2 style=\"text-align: center;\">Catálogo Curso "+<?php echo $_SESSION['exercicio']; ?>+"</h2>";
                	    jQuery('#top_content_label_sistema').html(exercicio);
                	});     
                </script>
<?php
        break;
endswitch;
?>