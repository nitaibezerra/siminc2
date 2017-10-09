<?php

include  APPRAIZ."includes/cabecalho.inc";

$titulo_modulo = "Medida Saneadora";
$modulo = "";
monta_titulo( $titulo_modulo, $modulo );

// dependências --------------------------------------------------------------------------------------------------
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "cproc/classes/ModeloCproc.class.inc";
include_once APPRAIZ . "cproc/classes/Processo.class.inc";
include_once APPRAIZ . "cproc/classes/WorkflowCproc.class.inc";
// --------------------------------------------------------------------------------------------------
include_once '../../cproc/classes/MedidaSaneadora.class.inc';

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




$MedidaSaneadora = new MedidaSaneadora();

if( $_GET['msaid'] ){

	if( $_GET['excluir'] ){
		$MedidaSaneadora->excluir( $_GET['msaid'] );
		if( $MedidaSaneadora->commit() ){
			echo "<script>
				alert('Elemento excluído com sucesso.');
				window.location.href = 'cproc.php?modulo=principal/medidasaneadora&acao=A';
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

	$MedidaSaneadora->carregar( $_GET['msaid'] );
}

if( $_POST['tipoForm'] ):

	if( $_POST['tipoForm'] == 'filtro' ){ // ------------------------------- filtro

		// -- filtros --
		$filtros = array();
		foreach( $_POST as $chave => $valor )
			if( $valor != '' )
				switch ($chave) {
					case 'msavigencia_filtro':
						$filtros['msavigencia'] = $valor;
						break;
					case 'msadatainicial_filtro':
						$filtros['msadatainicial'] = ajusta_data($valor);
						break;
					case 'msadatafinal_filtro':
						$filtros['msadatafinal'] = ajusta_data($valor);
						break;
					/*case 'msalinkdou_filtro':
						$filtros['msalinkdou'] = $valor;
						break;*/
				}
		// -- / filtros --

	}else if( $_POST['tipoForm']  == 'edicao' ){ // ------------------------------- edicao

		// recepcao de dados
		$campos = array();
		unset($campos['co_ies_campo']);
		unset($campos['co_mantenedora']);
		unset($campos['prcnumsidoc']);
		$campos['msaid'] = $_POST['msaid_campo'];
		$campos['prcid'] = $_POST['prcid_campo'];
		$campos['msavigencia'] = $_POST['msavigencia_campo'];
	    $campos['msadatainicial'] = ajusta_data($_POST['msadatainicial_campo']);
	    $campos['msadatalimite'] = ajusta_data($_POST['msadatalimite_campo']);
	    $campos['msadatafinal'] = ajusta_data($_POST['msadatafinal_campo']);
	    $campos['msaobservacoes'] = $_POST['msaobservacoes_campo'];
	    //$campos['msalinkdou'] = $_POST['msalinkdou_campo'];
	    // vigencia
			$campos['vmsjustificativa'] = $_POST['vmsjustificativa'];
    		$campos['vmsdtrevogacao'] = ajusta_data($_POST['vmsdtrevogacao']);
    		$campos['vmcdatanao'] = ajusta_data($_POST['vmcdatanao']);

	    if( !$campos['msaid'] ){ // insert

	    	// ver($campos,d);
	    	// $MedidaSaneadora->cadastrar($campos);
	    	if( $MedidaSaneadora->cadastrar($campos) ){
	    		echo "<script>
	    				alert('Cadastrado com sucesso.');
	    				window.location.href = 'cproc.php?modulo=principal/medidasaneadora&acao=A';
	    			</script> ";
	    		exit;
	    	}else{
	    		echo "<script>
	    				alert('Erro ao cadastrar.');
	    				window.location.href = 'cproc.php?modulo=principal/medidasaneadora&acao=A';
	    			</script> ";
	    		exit;
	    	}

	    }else{ // update

	    	$MedidaSaneadora->popula( $campos );
	    	if( $MedidaSaneadora->atualizar() ){

	    		// @internal campo antigo era 'msavigencia' e agora ficou... ou seja, 'msavigencia' é 'vmsvigencia'
	    		if(!$campos['msavigencia'])
	    			$campos['msavigencia'] = 'false';

	    		$dadosVigencia = array( 
	    			'vmsvigencia' => $campos['msavigencia'],
	    			'vmsid' => $MedidaSaneadora->vmsid,
	    			'vmsjustificativa' => $campos['vmsjustificativa'],
				    'vmsdtrevogacao' => $campos['vmsdtrevogacao'],
				    'vmcdatanao' => $campos['vmcdatanao'] );
	    		$MedidaSaneadora->atualizaVigencia( $dadosVigencia );

	    		echo " <script>
	    				alert('Atualizado com sucesso.');
	    				window.location.href = 'cproc.php?modulo=principal/medidasaneadora&acao=A&msaid=".($MedidaSaneadora->msaid)."';
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
	<h4>Dados da Medida Saneadora:</h4>
	
	<div>

		<form method="post" name="formMedidaSaneadora">
		<input name="tipoForm" id="tipoForm" value="edicao" type="hidden"/>
		<input name="msaid_campo" id="msaid_campo" type="hidden" value="<?=($MedidaSaneadora->msaid)?$MedidaSaneadora->msaid:''?>"/>
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
	                <?php
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
	        <?php responsaveisProcesso($prcid)?>
	        <tr style="height:20px;"></tr>
			<tr>
	            <td class="subtituloDireita" colspan="2">Vigência:</td>
	            <td>
	            	<?php 
	            	$vigencia = 't';
	            	$vmsjustificativa = '';
		            $vmsdtrevogacao = '';
		            $vmcdatanao = '';
	            	// dbg($MedidaSaneadora->vmsid != null,d);
	            	if( $MedidaSaneadora->vmsid != null ){
		            	$sql = "
		            		select 
		            			vmsvigencia,
		            			vmsjustificativa,
		            			vmsdtrevogacao,
		            			TO_CHAR(vmcdatanao,'DD/MM/YYYY') as vmcdatanao
		            		from cproc.vigenciamedsaneadora
		            		where vmsid = {$MedidaSaneadora->vmsid}
		            	";
		            	// ver($sql,d);
		            	$vigenciaCarregada = $db->carregar( $sql  );
		            	$vigencia = $vigenciaCarregada[0]['vmsvigencia'];
		            	$vmsjustificativa = $vigenciaCarregada[0]['vmsjustificativa'];
		            	$vmsdtrevogacao = $vigenciaCarregada[0]['vmsdtrevogacao'];
		            	$vmcdatanao = $vigenciaCarregada[0]['vmcdatanao'];
		            }
	            	?>
	                <input type="radio" name="msavigencia_campo" value="1" <?=($vigencia && $vigencia == 't')?'checked':''?> <?=($habil=='S')?'':'disabled' ?> />Sim
	                &nbsp;&nbsp;&nbsp;
	                <input type="radio" name="msavigencia_campo" value="0" <?=($vigencia && $vigencia == 'f')?'checked':''?> <?=($_GET['msaid'] || $habil=='S')?'':'disabled'?> />Não
	            </td>
	        </tr>
	        <tr id="linhaVigenciaJustificativa" style="<?=($vigencia=='t')?'display:none;':'display:;'?>">
	        	<td class="subtituloDireita" colspan="2">Justificativa da não vigência:</td>
	            <td>
	            	<textarea style="width:300px;height:150px" <?=($habil=='S')?'':'disabled' ?> maxlength="400" name="vmsjustificativa" id="vmsjustificativa"><?=$vmsjustificativa?></textarea>
	            </td>
	        </tr>
	        <tr id="linhaVigenciaDataRevogacao" style="<?=($vigencia=='t')?'display:none;':'display:;'?>">
	        	<td class="subtituloDireita" colspan="2">Data Revogação:</td>
	            <td>
	            	<?php
	            		echo campo_data2('vmsdtrevogacao','N',$habil,'Data Revogação','S','','', $vmsdtrevogacao);
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
	            <td class="subtituloDireita" colspan="2">Data Inicial:</td>
	            <td>
	                <?php 
	                	$msadatainicial = ($MedidaSaneadora->msadatainicial)?$MedidaSaneadora->msadatainicial:'';
	                	echo campo_data2('msadatainicial_campo', 'S', $habil, 'Prazo', 'S', '', '', $msadatainicial, '', '', 'msadatainicial_campo'); 
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data Limite:</td>
	            <td>
	                <?php 
	                	$msadatalimite = ($MedidaSaneadora->msadatalimite)?$MedidaSaneadora->msadatalimite:'';
	                	echo campo_data2('msadatalimite_campo', 'S', $habil, 'Prazo', 'S', '', '', $msadatalimite, '', '', 'msadatalimite_campo'); 
	                ?>
	            </td>
	        </tr>
	        <tr <?=($_GET['msaid'])?'':'style="display: none;"'?>>
	            <td class="subtituloDireita" colspan="2">Data Final:</td>
	            <td>
	                <?php 
	                	$msadatafinal = ($MedidaSaneadora->msadatafinal)?$MedidaSaneadora->msadatafinal:'';
	                	if (!empty($msadatafinal) || $_GET['msaid']){
	                		echo campo_data2('msadatafinal_campo', 'S', $habil, 'Prazo', 'S', '', '', $msadatafinal, '', '', 'msadatafinal_campo');
	                	} 
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Observações:</td>
	            <td>
	                <textarea style="width:300px;height:150px" maxlength="400" <?=($habil=='S')?'':'disabled' ?> name="msaobservacoes_campo" id="msaobservacoes_campo"><?=($MedidaSaneadora->msaobservacoes)?$MedidaSaneadora->msaobservacoes:'';?></textarea>
	            </td>
	        </tr>
	        <!-- tr>
	            <td class="subtituloDireita" colspan="2">Link DOU:</td>
	            <td>
	                <?php
	                	//$msalinkdou = ($MedidaSaneadora->msalinkdou)?$MedidaSaneadora->msalinkdou:'';
	                    //echo campo_texto('msalinkdou_campo', 'N', 'S', 'Link DOU', '51', '100', '', '', '', '', '', 'id="msalinkdou_campo"', '', $msalinkdou, '');
	                ?>
	            </td>
	        </tr -->
	        <tr>
	            <td class="SubTituloCentro" colspan="3">
	            	<?php if ($habil=='S') {?>
		                <input type="button" name="salvarProcesso" id="salvarProcesso" value="<?=( $_GET['msaid'] )?'Salvar':'Cadastrar';?>" onclick="salvar();"/>
		                &nbsp;&nbsp;&nbsp;
		                <input name="reset" type="reset" value="Limpar campos"/>
		                <?php if( $_GET['msaid'] ){ ?>
		                	&nbsp;&nbsp;&nbsp;
		                	<input name="novo" type="button" value="Cadastrar Nova Medida Saneadora" onclick="javascript:window.location.href='cproc.php?modulo=principal/medidasaneadora&acao=A'"/>
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
	            	<select name="msavigencia_filtro" id="msavigencia_filtro">
	            		<option value=""></option>
	            		<option value="t" <?=($filtros['msavigencia'] && $filtros['msavigencia']=='t')?'selected':''?> >Sim</option>
	            		<option value="f" <?=($filtros['msavigencia'] && $filtros['msavigencia']=='f')?'selected':''?> >Não</option>
	            	</select>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data Inicial:</td>
	            <td> 
	            	<?php 
	                	$msadatainicial_filtro = ($filtros['msadatainicial'])?$filtros['msadatainicial']:'';
	                	echo campo_data2('msadatainicial_filtro', 'S', 'S', 'Data Inicial', 'S', '', '', $msadatainicial_filtro, '', '', 'msadatainicial_filtro'); 
	                ?>
	           	</td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data Limite:</td>
	            <td> 
	            	<?php 
	                	$msadatalimite_filtro = ($filtros['msadatalimite'])?$filtros['msadatalimite']:'';
	                	echo campo_data2('msadatalimite_filtro', 'S', 'S', 'Data Limite', 'S', '', '', $msadatalimite_filtro, '', '', 'msadatalimite_filtro'); 
	                ?>
	           	</td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data Final:</td>
	            <td> 
	            	<?php 
	                	$msadatafinal_filtro = ($filtros['msadatafinal'])?$filtros['msadatafinal']:'';
	                	echo campo_data2('msadatafinal_filtro', 'S', 'S', 'Data Final', 'S', '', '', $msadatafinal_filtro, '', '', 'msadatafinal_filtro'); 
	                ?>
	           	</td>
	        </tr>
	        <!-- tr>
	            <td class="subtituloDireita" colspan="2">Link DOU:</td>
	            <td> <input name="msalinkdou_filtro" id="msalinkdou_filtro" value="<?=@$filtros['msalinkdou']?>" /> </td>
	        </tr -->
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
		$cabecalho = array("Ação","Vigência","Data Inicial", "Data Limite","Data Final");
		$alinhamento = array('left','left','left','left','left');
		$larguras = array('5%','5%','30%','30%','30%');
		$filtros['prcid'] = $processo['prcid'];
		// ver($MedidaSaneadora->montaListaQuery( $filtros ),d);
		$db->monta_lista($MedidaSaneadora->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaMedidaSaneadora', $larguras,$alinhamento);
		?>
	</div>

</div>

<script>

function salvar(){
	document.forms.formMedidaSaneadora.submit();
}

function editar( chave ){
	window.location.href = "cproc.php?modulo=principal/medidasaneadora&acao=A&msaid="+chave;
}

function excluir( chave ){
	if( confirm('Deseja apagar esse item?') ){
		window.location.href = "cproc.php?modulo=principal/medidasaneadora&acao=A&excluir=1&msaid="+chave;
	}
}

jQuery('document').ready(function(){

	// observer para o campo vigencia
	jQuery('[name="msavigencia_campo"]').click(function(){

		if( jQuery('[name="msavigencia_campo"]:checked').val() == 0 ){ // não

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