<?php

include  APPRAIZ."includes/cabecalho.inc";

$titulo_modulo = "Ministério Público";
$modulo = "";
monta_titulo( $titulo_modulo, $modulo );

// dependências --------------------------------------------------------------------------------------------------
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "cproc/classes/ModeloCproc.class.inc";
include_once APPRAIZ . "cproc/classes/Processo.class.inc";
include_once APPRAIZ . "cproc/classes/WorkflowCproc.class.inc";
// --------------------------------------------------------------------------------------------------
include_once '../../cproc/classes/MinisterioPublico.class.inc';

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




$MinisterioPublico = new MinisterioPublico();

if( $_GET['mpuid'] ){

	if( $_GET['excluir'] ){
		$MinisterioPublico->excluir( $_GET['mpuid'] );
		if( $MinisterioPublico->commit() ){
			echo "<script>
				alert('Elemento excluÃ­do com sucesso.');
				window.location.href = 'cproc.php?modulo=principal/ministeriopublico&acao=A';
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

	$MinisterioPublico->carregar( $_GET['mpuid'] );
}

if( $_POST['tipoForm'] ):

	if( $_POST['tipoForm'] == 'filtro' ){ // ------------------------------- filtro

		// -- filtros --
		$filtros = array();
		foreach( $_POST as $chave => $valor )
			if( $valor != '' )
				switch ($chave) {
					case 'pmpvigencia_filtro':
						$filtros['pmpvigencia'] = $valor;
						break;
					case 'mpuindicarsidoc_filtro':
						$filtros['mpuindicarsidoc'] = $valor;
						break;
				}
		// -- / filtros --

	}else if( $_POST['tipoForm']  == 'edicao' ){ // ------------------------------- edicao

		// recepcao de dados
		$campos = array();
		unset($campos['co_ies_campo']);
		unset($campos['co_mantenedora']);
		unset($campos['prcnumsidoc']);
		$campos['mpuid'] = $_POST['mpuid_campo'];
		$campos['prcid'] = $_POST['prcid_campo'];
		$campos['pmpvigencia'] = $_POST['pmpvigencia_campo'];
	    $campos['mpuindicarsidoc'] = $_POST['mpuindicarsidoc_campo'];
	    $campos['mpuobservacoes'] = $_POST['mpuobservacoes_campo'];
	    // vigencia
			$campos['pmpjustificativa'] = $_POST['pmpjustificativa'];
    		$campos['pmpdtrevogacao'] = ajusta_data($_POST['pmpdtrevogacao']);
    		$campos['pmpdatanao'] = ajusta_data($_POST['pmpdatanao']);

	    if( !$campos['mpuid'] ){ // insert

	    	// ver($campos,d);
	    	// $MinisterioPublico->cadastrar($campos);
	    	if( $MinisterioPublico->cadastrar($campos) ){
	    		echo "<script>
	    				alert('Cadastrado com sucesso.');
	    				window.location.href = 'cproc.php?modulo=principal/ministeriopublico&acao=A';
	    			</script> ";
	    		exit;
	    	}else{
	    		echo "<script>
	    				alert('Erro ao cadastrar.');
	    				window.location.href = 'cproc.php?modulo=principal/ministeriopublico&acao=A';
	    			</script> ";
	    		exit;
	    	}

	    }else{ // update

	    	$MinisterioPublico->popula( $campos );
	    	if( $MinisterioPublico->atualizar() ){

	    		if(!$campos['pmpvigencia'])
	    			$campos['pmpvigencia'] = 'false';

	    		$dadosVigencia = array( 
	    			'pmpvigencia' => $campos['pmpvigencia'],
	    			'pmpid' => $MinisterioPublico->pmpid,
	    			'pmpjustificativa' => $campos['pmpjustificativa'],
				    'pmpdtrevogacao' => $campos['pmpdtrevogacao'],
				    'pmpdatanao' => $campos['pmpdatanao'] );
	    		$MinisterioPublico->atualizaVigencia( $dadosVigencia );

	    		echo " <script>
	    				alert('Atualizado com sucesso.');
	    				window.location.href = 'cproc.php?modulo=principal/ministeriopublico&acao=A&mpuid=".($MinisterioPublico->mpuid)."';
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
	<h4>Dados da Ministério Público:</h4>
	
	<div>

		<form method="post" name="formMinisterioPublico">
		<input name="tipoForm" id="tipoForm" value="edicao" type="hidden"/>
		<input name="mpuid_campo" id="mpuid_campo" type="hidden" value="<?=($MinisterioPublico->mpuid)?$MinisterioPublico->mpuid:''?>"/>
		<input name="prcid_campo" id="prcid_campo" type="hidden" value="<?=$processo['prcid']?>"/>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
	        <tr>
	            <td class="subtituloDireita" colspan="2">Nº do Processo:</td>
	            <td>
	                <?php
	                    echo campo_texto('prcnumsidoc_campo', 'S', 'N', 'Nº Controle SIMEC', '51', '25', '####.######/####-##', '', '', '', '', 'id="prcid_campo"', '', $processo['prcnumsidoc'], '');
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
	        	        	        
            <?php responsaveisProcesso($prcid)?>
	        
	        <tr style="height:20px;"></tr>

			<tr>
	            <td class="subtituloDireita" colspan="2">Pendência:</td>
	            <td>
	            	<?php 
	            	$vigencia = 't';
	            	$pmpjustificativa = '';
		            $pmpdtrevogacao = '';
		            $pmpdatanao = '';
	            	if( $MinisterioPublico->pmpid != null ){
		            	$sql = "
		            		select 
		            			pmpvigencia,
		            			pmpjustificativa,
		            			pmpdtrevogacao,
		            			TO_CHAR(pmpdatanao,'DD/MM/YYYY') as pmpdatanao
		            		from cproc.pendenciaministpublico
		            		where pmpid = {$MinisterioPublico->pmpid}
		            	";
		            	// ver($sql,d);
		            	$vigenciaCarregada = $db->carregar( $sql  );
		            	// ver($vigenciaCarregada,d);
		            	$vigencia = $vigenciaCarregada[0]['pmpvigencia'];
		            	$pmpjustificativa = $vigenciaCarregada[0]['pmpjustificativa'];
		            	$pmpdtrevogacao = $vigenciaCarregada[0]['pmpdtrevogacao'];
		            	$pmpdatanao = $vigenciaCarregada[0]['pmpdatanao'];
		            }
	            	?>
	                <input type="radio" name="pmpvigencia_campo" value="1" <?=($vigencia && $vigencia == 't')?'checked':''?> <?=($habil=='S')?'':'disabled' ?> />Sim
	                &nbsp;&nbsp;&nbsp;
	                <input type="radio" name="pmpvigencia_campo" value="0" <?=($vigencia && $vigencia == 'f')?'checked':''?> <?=($_GET['mpuid'] || $habil=='S')?'':'disabled'?> />Não
	            </td>
	        </tr>
	        <tr id="linhaVigenciaJustificativa" style="<?=($vigencia=='t')?'display:none;':'display:;'?>">
	        	<td class="subtituloDireita" colspan="2">Justificativa da não vigência:</td>
	            <td>
	            	<textarea style="width:300px;height:150px" <?=($habil=='S')?'':'disabled' ?> maxlength="400" name="pmpjustificativa" id="pmpjustificativa"><?=$pmpjustificativa?></textarea>
	            </td>
	        </tr>
	        <tr id="linhaVigenciaDataRevogacao" style="<?=($vigencia=='t')?'display:none;':'display:;'?>">
	        	<td class="subtituloDireita" colspan="2">Data de Atendimento:</td>
	            <td>
	            	<?php
	            		echo campo_data2('pmpdtrevogacao','N',$habil,'Data de Atendimento','S','','', $pmpdtrevogacao);
	            	?>
	            </td>
	        </tr>
	        <tr id="linhaVigenciaDataNao" style="<?=($vigencia=='t')?'display:none;':'display:;'?>">
	        	<td class="subtituloDireita" colspan="2">Data Não:</td>
	            <td>
	            	<?=$pmpdatanao?>
	            </td>
	        </tr>

	        <tr style="height:20px;"></tr>

	        <tr>
	            <td class="subtituloDireita" colspan="2">SIDOC:</td>
	            <td>
	                <?php
	                	$mpuindicarsidoc = ($MinisterioPublico->mpuindicarsidoc)?$MinisterioPublico->mpuindicarsidoc:'';
	                    echo campo_texto('mpuindicarsidoc_campo', 'S', $habil, 'SIDOC', '51', '20', '#####.######/####-##', '', '', '', '', 'id="mpuindicarsidoc_campo"', '', $mpuindicarsidoc, '');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Observações:</td>
	            <td>
	                <textarea style="width:300px;height:150px" <?=($habil=='S')?'':'disabled' ?> maxlength="400" name="mpuobservacoes_campo" id="mpuobservacoes_campo"><?=($MinisterioPublico->mpuobservacoes)?$MinisterioPublico->mpuobservacoes:'';?></textarea>
	            </td>
	        </tr>
	        <tr>
	            <td class="SubTituloCentro" colspan="3">
	            	<?php if($habil=='S') { ?>
		                <input type="button" name="salvarProcesso" id="salvarProcesso" value="<?=( $_GET['mpuid'] )?'Salvar':'Cadastrar';?>" onclick="salvar();"/>
		                &nbsp;&nbsp;&nbsp;
		                <input name="reset" type="reset" value="Limpar campos"/>
		                <?php if( $_GET['mpuid'] ){ ?>
		                	&nbsp;&nbsp;&nbsp;
		                	<input name="novo" type="button" value="Cadastrar Nova Ministério Público" onclick="javascript:window.location.href='cproc.php?modulo=principal/ministeriopublico&acao=A'"/>
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
	            <td class="subtituloDireita" colspan="2">Pendência:</td>
	            <td> 
	            	<select name="pmpvigencia_filtro" id="pmpvigencia_filtro">
	            		<option value=""></option>
	            		<option value="t" <?=($filtros['pmpvigencia'] && $filtros['pmpvigencia']=='t')?'selected':''?> >Sim</option>
	            		<option value="f" <?=($filtros['pmpvigencia'] && $filtros['pmpvigencia']=='f')?'selected':''?> >Não</option>
	            	</select>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">SIDOC:</td>
	            <td>
	            	<?php
	            		$mpuindicarsidoc_filtro = ($filtros['mpuindicarsidoc'])?$filtros['mpuindicarsidoc']:'';
	                    echo campo_texto('mpuindicarsidoc_filtro', 'S', 'S', 'SIDOC', '51', '25', '##.######/####-##', '', '', '', '', 'id="mpuindicarsidoc_filtro"', '', $mpuindicarsidoc_filtro, '');
	                ?>
	            </td>
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
		$cabecalho = array("Ação","Pendência","SIDOC");
		$alinhamento = array('left','left','left');
		$larguras = array('10%','5%','40%','40%');
		// $filtros['prcid'] = $processo['prcid'];
		// ver($MinisterioPublico->montaListaQuery( $filtros ),d);
		$db->monta_lista($MinisterioPublico->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaMinisterioPublico', $larguras,$alinhamento);
		?>
	</div>

</div>

<script>

function salvar(){

	if (document.getElementById('mpuindicarsidoc_campo').value.length < 12) {
		alert('SIDOC deve conter, no mínimo, 12 caractéres.');
		return false;
	}
	
	document.forms.formMinisterioPublico.submit();
}

function editar( chave ){
	window.location.href = "cproc.php?modulo=principal/ministeriopublico&acao=A&mpuid="+chave;
}

function excluir( chave ){
	if( confirm('Deseja apagar esse item?') ){
		window.location.href = "cproc.php?modulo=principal/ministeriopublico&acao=A&excluir=1&mpuid="+chave;
	}
}

jQuery('document').ready(function(){

	// observer para o campo vigencia
	jQuery('[name="pmpvigencia_campo"]').click(function(){

		if( jQuery('[name="pmpvigencia_campo"]:checked').val() == 0 ){ // não

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