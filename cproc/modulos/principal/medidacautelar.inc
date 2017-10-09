<?php

include  APPRAIZ."includes/cabecalho.inc";

$titulo_modulo = "Medida Cautelar";
$modulo = "";
monta_titulo( $titulo_modulo, $modulo );

// dependências --------------------------------------------------------------------------------------------------
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "cproc/classes/ModeloCproc.class.inc";
include_once APPRAIZ . "cproc/classes/Processo.class.inc";
include_once APPRAIZ . "cproc/classes/WorkflowCproc.class.inc";
// --------------------------------------------------------------------------------------------------
include_once '../../cproc/classes/MedidaCautelar.class.inc';

if( !verificaExistenciaPrcid() ){
	echo "
		<script>
			alert('Sessão perdeu processo.');
			window.location.href = 'cproc.php?modulo=principal/lista_grid_processo&acao=A';
		</script>";
	exit;	
}
 if ($_SESSION['cproc']['prcid']) {
         $processo = exibirProcesso($_SESSION['cproc']['prcid']);
         extract($processo);
    }
    else{
        $processo = exibirProcesso($prcid);
    }


$MedidaCautelar = new MedidaCautelar();

if( $_GET['mdcid'] ){

	if( $_GET['excluir'] ){
		$MedidaCautelar->excluir( $_GET['mdcid'] );
		if( $MedidaCautelar->commit() ){
			echo "<script>
				alert('Elemento excluído com sucesso.');
				window.location.href = 'cproc.php?modulo=principal/medidacautelar&acao=A';
			</script>";
			exit;
		}else{
			echo "<script>
				alert('Erro ao tentar excluir elemento.');
				window.history.back();
			</script>";
			exit;
		}
	}

	$MedidaCautelar->carregar( $_GET['mdcid'] );
}

if( $_POST['tipoForm'] ):

	if( $_POST['tipoForm'] == 'filtro' ){ // ------------------------------- filtro

		// -- filtros --
		$filtros = array();
		foreach( $_POST as $chave => $valor )
			if( $valor != '' )
				switch ($chave) {
					case 'mdcvigencia_filtro':
						$filtros['vmcvigencia'] = $valor;
						break;
					case 'mdctipo_filtro':
						$filtros['mdctipo'] = $valor;
						break;
					case 'mdcdatainicial_filtro':
						$filtros['mdcdatainicial'] = ajusta_data($valor);
						break;
					case 'mdcdatafinal_filtro':
						$filtros['mdcdatafinal'] = ajusta_data($valor);
						break;
				}
		// -- / filtros --

	}else if( $_POST['tipoForm']  == 'edicao' ){ // ------------------------------- edicao

		// recepcao de dados
		$campos = array();
		unset($campos['co_ies_campo']);
		unset($campos['co_mantenedora_campo']);
		unset($campos['prcnumsidoc_campo']);
		// medida cautelar
			$campos['mdcid'] = $_POST['mdcid_campo'];
			$campos['prcid'] = $_POST['prcid_campo'];
			$campos['mdcvigencia'] = $_POST['mdcvigencia_campo'];
		    $campos['mdcobservacoes'] = $_POST['mdcobservacoes_campo'];
		    $campos['mdclinkdou'] = $_POST['mdclinkdou_campo'];
		    $campos['mdcdatafinal'] = ajusta_data($_POST['mdcdatafinal']);
		    $campos['mdcdatainicial'] = ajusta_data($_POST['mdcdatainicial']);
		// vigencia
			$campos['vmcjustificativa'] = $_POST['vmcjustificativa'];
    		$campos['vmcdtrevogacao'] = ajusta_data($_POST['vmcdtrevogacao']);
    		$campos['vmcdatanao'] = ajusta_data($_POST['vmcdatanao']);
    	// objeto medida cautelar
        	$campos['omcid'] = $_POST['omcid'];


	    if( !$campos['mdcid'] ){ // insert

	    	// ver($campos,d);
	    	if( $MedidaCautelar->cadastrar( $campos ) ){
	    		echo "<script>
	    				alert('Cadastrado com sucesso.');
	    				window.location.href = 'cproc.php?modulo=principal/medidacautelar&acao=A';
	    			</script> ";
	    		exit;
	    	}else{
	    		echo "<script>
	    				alert('Erro ao cadastrar.');
	    				window.location.href = 'cproc.php?modulo=principal/medidacautelar&acao=A';
	    			</script> ";
	    		exit;
	    	}

	    }else{ // update

	    	$MedidaCautelar->popula( $campos );
	    	if( $MedidaCautelar->atualizar() ){
	    		
	    		// @internal campo antigo era 'mdcvigencia' e agora ficou... ou seja, 'mdcvigencia' é 'vmcvigencia'
	    		if(!$campos['mdcvigencia']){
	    			$campos['mdcvigencia'] = 'false';
    			}
	    		else {
	    			// Este tratamento foi necessário porque o banco não estava reconhecendo o valor retornado pelo campo como booleano
	    			switch ($campos['mdcvigencia']){
	    				case 1:
	    					$campos['mdcvigencia'] = 'true';
	    					break;
	    				case 0:
	    					$campos['mdcvigencia'] = 'false';
	    					break;
	    			}
	    		}
				
	    		if ($campos['vmcdtrevogacao'] == '--'){
	    			$campos['vmcdtrevogacao'] = null;
	    		}
	    		
	    		$dadosVigencia = array( 
	    			'vmcvigencia' => $campos['mdcvigencia'],
	    			'vmcid' => $MedidaCautelar->vmcid,
	    			'vmcjustificativa' => $campos['vmcjustificativa'],
				    'vmcdtrevogacao' => $campos['vmcdtrevogacao'],
				    'vmcdatanao' => $campos['vmcdatanao'] );

	    		$MedidaCautelar->atualizaVigencia( $dadosVigencia );

	    		echo " <script>
	    				alert('Atualizado com sucesso.');
	    				window.location.href = 'cproc.php?modulo=principal/medidacautelar&acao=A&mdcid=".($MedidaCautelar->mdcid)."';
	    			</script> ";
	    		exit;
	    	}else{
	    		echo " <script>
	    				alert('Existem campos obrigatórios em branco.');
	    				window.history.back();
	    			</script> ";
	    		exit;
	    	}
	    }

	}

endif;

$habil = retornaPermissaoDetalhe($prcid);
//$habil = 'S';

?>

<br>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript">
	jQuery.noConflict();
	
	jQuery('.adicionar').live('click',function(){
		return window.open('assint.php?modulo=principal/cadCursoPEC_G&acao=A','modelo',"height=450,width=850,scrollbars=yes,top=50,left=200");
	});
</script>

<?php 
$arMnuid = array();
$db->cria_aba( ABA_CAD_PROCESSO, $url, '', $arMnuid );
?>

<div style="width:98%;margin:0 auto;">
	<h4>Dados da Medida Cautelar:</h4>
	
	<div>

		<form method="post" name="formMedidaCautelar">
		<input name="tipoForm" id="tipoForm" value="edicao" type="hidden"/>
		<input name="mdcid_campo" id="mdcid_campo" type="hidden" value="<?=($MedidaCautelar->mdcid)?$MedidaCautelar->mdcid:''?>"/>
		<input name="prcid_campo" id="prcid_campo" type="hidden" value="<?=$processo['prcid']?>"/>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
	        <tr>
	            <td class="subtituloDireita" colspan="2">Nº do Processo:</td>
	            <td>
	                <?php
	                    echo campo_texto('prcnumsidoc_campo', 'S', 'N', 'Nº Controle ' SIGLA_SISTEMA, '51', '25', '####.######/####-##', '', '', '', '', 'id="prcid_campo"', '', $processo['prcnumsidoc'], '');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Mantenedora:</td>
	            <td>
	                <?PHP
	                    $sql = "
	                        SELECT  co_mantenedora AS codigo,
	                                no_razao_social AS descricao
	                        FROM emec.mantenedora
	                        ORDER BY no_razao_social
	                    ";
	                    $db->monta_combo('mntid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['co_mantenedora'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Mantida:</td>
	            <td>
	                <?PHP
	                   	$sql = "
	                        SELECT  co_ies AS codigo,
	                                no_ies AS descricao
	                        FROM emec.ies
	                        ORDER BY no_ies
	                    ";
	                    $db->monta_combo('iesid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['co_ies'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Entidade:</td>
	            <td>
	                <?PHP
	                   	$sql = "
	                        SELECT  entid AS codigo,
	                                entdsc AS descricao
	                        FROM cproc.entidade
	                        ORDER BY entid
	                    ";
	                    $db->monta_combo('entid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['entid'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
	        
	        <tr style="height:20px;"></tr>

            <?php responsaveisProcesso($prcid)?>

	        <tr style="height:20px;"></tr>

			<tr>
	            <td class="subtituloDireita" colspan="2">Vigência:</td>
	            <td>
	            	<?php 
	            	// busca vigencia
	            	$vigencia = 't';
	            	$vmcjustificativa = '';
	            	$vmcdtrevogacao = '';
	            	$vmcdatanao = '';
	            	if( $MedidaCautelar->vmcid ){
		            	$sql = "
		            		SELECT 
		            			vig.vmcjustificativa as vmcjustificativa,
		            			vig.vmcvigencia as vmcvigencia,
		            			vig.vmcdtrevogacao as vmcdtrevogacao,
		            			TO_CHAR(vig.vmcdatanao,'DD/MM/YYYY') as vmcdatanao
		            		FROM cproc.vigenciamedcautelar vig
		            		WHERE vig.vmcid = {$MedidaCautelar->vmcid}
		            	";
		            	$resultado = $db->carregar($sql);
		            	$vigencia = $resultado[0]['vmcvigencia'];
		            	$vmcjustificativa = $resultado[0]['vmcjustificativa'];
		            	$vmcdtrevogacao = $resultado[0]['vmcdtrevogacao'];
		            	$vmcdatanao = $resultado[0]['vmcdatanao'];
		            }
	            	?>
	                <input type="radio" name="mdcvigencia_campo" value="1" <?=($vigencia && $vigencia == 't')?'checked':''?> <?=($habil=='S')?'':'disabled' ?> />Sim
	                &nbsp;&nbsp;&nbsp;
	                <input type="radio" name="mdcvigencia_campo" value="0" <?=($vigencia && $vigencia == 'f')?'checked':''?> <?=($_GET['mdcid'] || $habil=='S')?'':'disabled'?> />Não
	            </td>
	        </tr>
	        <tr id="linhaVigenciaJustificativa" style="<?=($vigencia=='t')?'display:none;':'display:;'?>">
	        	<td class="subtituloDireita" colspan="2">Justificativa da não vigência:</td>
	            <td>
	            	<textarea style="width:300px;height:150px" maxlength="400" name="vmcjustificativa" id="vmcjustificativa" ><?=$vmcjustificativa?></textarea>
	            </td>
	        </tr>
	        <tr id="linhaVigenciaDataRevogacao" style="<?=($vigencia=='t')?'display:none;':'display:;'?>">
	        	<td class="subtituloDireita" colspan="2">Data Revogação:</td>
	            <td>
	            	<?php
	            		echo campo_data2('vmcdtrevogacao','N','S','Data Revogação','S','','', $vmcdtrevogacao);
	            	?>
	            </td>
	        </tr>
	        <tr id="linhaVigenciaDataNao" style="<?=($vigencia=='t')?'display:none;':'display:;'?>">
	        	<td class="subtituloDireita" colspan="2">Data Não:</td>
	            <td>
	            	<?=$vmcdatanao?>
	            </td>
	        </tr>
	        

	        <tr style="height:20px;"></tr>

            <tr>
                <td class="subtituloDireita" colspan="2">Objeto medida cautelar:</td>
                <td id="td_municipio">
                    <?PHP 
                        $sql = "
                            SELECT  omcid AS codigo, 
                                    omcdsc AS descricao 
                            FROM cproc.objetomedcautelar 

                            WHERE omcstatus = 'A' 

                        "; 
                        $db->monta_combo('omcid', $sql, $habil, 'Selecione...', '', '', '', '455', 'N', '', '', $MedidaCautelar->omcid); 
                    ?>
                </td>
            </tr>
                
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data Inicial:</td>
	            <td>
	                <?php 
	                	$mdcdatainicial = ($MedidaCautelar->mdcdatainicial)?$MedidaCautelar->mdcdatainicial:'';
	                	echo campo_data2('mdcdatainicial', 'S', $habil, 'Data Inicial', 'S', '', '', $mdcdatainicial, '', '', 'mdcdatainicial'); 
	                ?>
	            </td>
	        </tr>
	        <tr <?=(!empty($mdcdatafinal) || $_GET['mdcid'])?'':'style="display: none;"'?>>
	            <td class="subtituloDireita" colspan="2">Data Final:</td>
	            <td>
	                <?php 
	                	$mdcdatafinal = ($MedidaCautelar->mdcdatafinal)?$MedidaCautelar->mdcdatafinal:'';
	                	if (!empty($mdcdatafinal) || $_GET['mdcid']) {
	                		echo campo_data2('mdcdatafinal', 'S', $habil, 'Data Final', 'S', '', '', $mdcdatafinal, '', '', 'mdcdatafinal');
	                	} 
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Observações:</td>
	            <td>
	                <textarea style="width:300px;height:150px" maxlength="400" name="mdcobservacoes_campo" id="mdcobservacoes_campo" <?=($habil=='S')?'':'disabled' ?>><?=($MedidaCautelar->mdcobservacoes)?$MedidaCautelar->mdcobservacoes:'';?></textarea>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Link DOU:</td>
	            <td>
	                <?php
	                	$mdclinkdou = ($MedidaCautelar->mdclinkdou)?$MedidaCautelar->mdclinkdou:'';
	                    echo campo_texto('mdclinkdou_campo', 'N', 'S', 'Link DOU', '51', '100', '', '', '', '', '', 'id="mdclinkdou_campo"', '', $mdclinkdou, '');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="SubTituloCentro" colspan="3">
	            	<?php if ($habil == 'S'){ ?>
		                <input type="button" name="salvarProcesso" id="salvarProcesso" value="<?=( $_GET['mdcid'] )?'Salvar':'Cadastrar';?>" onclick="salvar();"/>
		                &nbsp;&nbsp;&nbsp;
		                <input name="reset" type="reset" value="Limpar campos"/>
		                <?php if( $_GET['mdcid'] ){ ?>
		                	&nbsp;&nbsp;&nbsp;
		                	<input name="novo" type="button" value="Cadastrar Nova Medida Cautelar" onclick="javascript:window.location.href='cproc.php?modulo=principal/medidacautelar&acao=A'"/>
		                <?php } ?>
	                <?php } ?>
	            </td>
	        </tr>
	    </table>
		</form>

	</div>

	<hr/>

	<div>
		<h4>Lista:</h4>
		<form method="post" name="form_filtro">
		<input name="tipoForm" id="tipoForm" value="filtro" type="hidden"/>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
			<tr><td colspan="3"><h5>Filtros</h5></td></tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Vigência:</td>
	            <td> 
	            	<select name="mdcvigencia_filtro" id="mdcvigencia_filtro">
	            		<option value=""></option>
	            		<option value="t" <?=($filtros['vmcvigencia'] && $filtros['vmcvigencia']=='t')?'selected':''?> >Sim</option>
	            		<option value="f" <?=($filtros['vmcvigencia'] && $filtros['vmcvigencia']=='f')?'selected':''?> >Não</option>
	            	</select>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data inicial:</td>
	            <td> 
	            	<?php 
	                	$mdcdatainicial_filtro = ($filtros['mdcdatainicial'])?$filtros['mdcdatainicial']:'';
	                	echo campo_data2('mdcdatainicial_filtro', 'S', 'S', 'Data Final', 'S', '', '', $mdcdatainicial_filtro, '', '', 'mdcdatainicial_filtro'); 
	                ?>
	           	</td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data final:</td>
	            <td> 
	            	<?php 
	                	$mdcdatafinal_filtro = ($filtros['mdcdatafinal'])?$filtros['mdcdatafinal']:'';
	                	echo campo_data2('mdcdatafinal_filtro', 'S', 'S', 'Data Final', 'S', '', '', $mdcdatafinal_filtro, '', '', 'mdcdatafinal_filtro'); 
	                ?>
	           	</td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Link DOU:</td>
	            <td> <input name="mdclinkdou_filtro" id="mdclinkdou_filtro" value="<?=@$filtros['mdclinkdou']?>" /> </td>
	        </tr>
	        <tr>
	            <td class="SubTituloCentro" colspan="3">
	                <input type="submit" name="buscar" id="buscar" value="Buscar"/>
	                &nbsp;&nbsp;&nbsp;
	                <input name="reset" type="reset" value="Limpar campos"/>
	            </td>
	        </tr>
	    </table>
		</form>

		<?php 

		$cabecalho = array("Ação","Vigência","Data Inicial","Data Final","Link Diário Oficial");
		$alinhamento = array('left','left','left','left','left','left');
		$larguras = array('5%','5%','20%','20%','20%','30%');
		$filtros['prcid'] = $processo['prcid'];
		// ver($MedidaCautelar->montaListaQuery( $filtros ),d);
		$db->monta_lista($MedidaCautelar->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaMedidaCautelar', $larguras,$alinhamento);
		?>
	</div>

</div>

<script>

function salvar(){

	if (jQuery('[name="omcid"]').val() == ''){
		alert('Informe o Objeto medida cautelar.');
		return;
	}

	// Valida se é uma edição ou um cadastro de uma nova medida cautelar
	var mdcid = '<?= $_GET['mdcid'] ?>';
	if (mdcid != '') {
		// Se for uma edição o campo 'mdcdatafinal' é obrigatório.
		if (document.getElementById('mdcdatafinal').value == ''){
			alert('Informe a Data Final.');
			return;
		}
	}	
	
	if (document.getElementById('mdcdatainicial').value == ''){
		alert('Informe a Data Inicial.');
		return;
	}
	
	document.forms.formMedidaCautelar.submit();
}

function editar( chave ){
	window.location.href = "cproc.php?modulo=principal/medidacautelar&acao=A&mdcid="+chave;
}

function excluir( chave ){
	if( confirm('Deseja apagar esse item?') ){
		window.location.href = "cproc.php?modulo=principal/medidacautelar&acao=A&excluir=1&mdcid="+chave;
	}
}

jQuery('document').ready(function(){

	// observer para o campo vigencia
	jQuery('[name="mdcvigencia_campo"]').click(function(){

		if( jQuery('[name="mdcvigencia_campo"]:checked').val() == 0 ){ // não

			jQuery('#linhaVigenciaJustificativa').css('display','');
			jQuery('#linhaVigenciaDataRevogacao').css('display','');
			jQuery('#linhaVigenciaDataNao').css('display','');
		}else{// sim
			
			jQuery('#linhaVigenciaJustificativa').css('display','none');
			jQuery('#linhaVigenciaDataRevogacao').css('display','none');
			jQuery('#linhaVigenciaDataNao').css('display','none');
		}
	});
});

</script>