<?php

if($_REQUEST['excluir']){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql = "DELETE FROM emenda.execucaofinanceira WHERE exfid = ".$_POST['exfid'];
	$db->executar( $sql );
	$db->commit();
	exit;
}

switch ( $_REQUEST["requisicao"] ){
	case 'salvaranalise':
		salvarAnaliseGestor( $_REQUEST );
	break;

}

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];
$ptrid = $_SESSION["emenda"]["ptridAnalise"];

if ( !$ptrid ){
	echo '<script>
			alert("A sessão do Plano de Trabalho foi perdida!");
			window.location = "emenda.php?modulo=principal/analiseDadosPTA&acao=A";
		  </script>';
	die;
}

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);

monta_titulo( 'Análise do Gestor', $subtitulo_modulo );

if ( $ptrid ){
	
	echo cabecalhoPlanoTrabalho($ptrid, true);
	$docid = criarDocumento( $ptrid );
	$existePendencias = pegaConclusaoAnaliseDados( $ptrid, 'M' );
	
	$arDados = $db->pegaLinha("SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
	
	if( $arDados['ptrnumprocessoempenho'] && $arDados['ptrnumdocumenta'] ){
		$arDados['ptrnumprocessoempenho'] = substr($arDados['ptrnumprocessoempenho'],0,5) . "." .
											substr($arDados['ptrnumprocessoempenho'],5,6) . "/" .
											substr($arDados['ptrnumprocessoempenho'],11,4) . "-" .
											substr($arDados['ptrnumprocessoempenho'],15,2);
									 
		$arDados['ptrnumdocumenta'] = substr($arDados['ptrnumdocumenta'],0,7) . "/" .
									  substr($arDados['ptrnumdocumenta'],7,4) . "-" .
									  substr($arDados['ptrnumdocumenta'],11,2);
	}
							
	
	extract($arDados);
	
	montaVisualizarPTA( $ptrid, true, $boReformulacao );
	abrePTAAnaliseRelatorio($ptrid, 'T', $boReformulacao, false);
	
	//$arDadosExef = carregaExecucaoOrcamentaria();
	
	//$boExeOrc = ($arDadosExef ? true : false);
	
	montaExecucaoOrcamentaria($ptrid, $boReformulacao);
	
}

?>

<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			
		</td>
	</tr>
</table>
<script><!--
	
	function salvaAnaliseGestor(){
		var ptrnumprocessoempenho = document.getElementById('ptrnumprocessoempenho').value;
		var ptrnumdocumenta 	  = document.getElementById('ptrnumdocumenta').value;

		if( ptrnumprocessoempenho != '' || ptrnumdocumenta != ''){
			document.getElementById('formulario').submit();
		}
	}
	
	function carregarRecurso( ptres, pi, exfvalor, exfcod, exfnumempenhooriginal, emeid ){

		var tabela 	= document.getElementById( "tabelaExecucaoOrcamentaria_" + emeid );
		var tbody   = document.getElementById( "tbodyTabela_" + emeid );
		var qtdLinha = tabela.rows.length;
		var qtd = qtdLinha - 1;
		var qtdL = qtd - 1;
		var boExcluir = true;
										
		linha = tabela.insertRow( tabela.rows.length );
		tbody.appendChild( linha );
	
		linha.setAttribute( "id", "linha_" + qtd );
		linha.setAttribute( "style", "background: #f5f5f5;" );
		linha.setAttribute( "align", "center" );
												
		var img = document.createElement( "img" );
		img.setAttribute( "src", "/imagens/excluir.gif" );
		img.setAttribute( "title", "Excluir" );
		img.setAttribute( "alt", "Excluir" );

		// OnClick
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			img.setAttribute( "onclick", "removerLinha( 'linha_" + qtd + "', '"+ emeid +"' )" );
		}
		else{ // IE
			img.attachEvent( "onclick", function() { removerLinha( "linha_" + qtd, emeid ) } );
		}
		
		var selectPTRES = document.createElement( 'select' );
		selectPTRES.setAttribute('style','width:50%');
		selectPTRES.setAttribute('name','ptres['+emeid+'][]');
		selectPTRES.setAttribute('id','ptres[]');
		if( exfnumempenhooriginal ){
			selectPTRES.setAttribute('disabled','disabled');
		}
		
		
		<?php 
		$arPTRES = $_SESSION['emenda']['arPtres'];
		$arPTRES = $arPTRES ? $arPTRES : array();
		foreach ($arPTRES as $key => $val){
			$arValor = $_SESSION['emenda']['acaid']."|".$val['codigo']; 
			?>			
			var option = document.createElement( 'option' );
			option.setAttribute( 'value', '<?php echo $arValor; ?>' );
				
				if( ptres ){
					if( ptres == '<?php echo $val['pliptres']; ?>' ){
						option.setAttribute( 'selected', 'selected' );
						boExcluir = false;
					}
				}
				option.innerHTML = '<?php echo $val["descricao"]; ?>';
			selectPTRES.appendChild( option );				
		<?php } ?>	
		var selectPI = document.createElement( 'select' );
		selectPI.setAttribute('style','width:80%');
		selectPI.setAttribute('class','CampoEstilo');
		selectPI.setAttribute('name','pi['+emeid+'][]');
		selectPI.setAttribute('id','pi['+ qtdL +']');
		if( exfnumempenhooriginal ){
			selectPI.setAttribute('disabled','disabled');
		}
		
		<?php 
		if( empty($_SESSION['emenda']['planointerno']) ){
			if( !empty($arValor) ){
				$arPI = recuperaSQLComboPTRES($arValor);	
			}
		} else {
			$arPI = $_SESSION['emenda']['planointerno'];
		}	
		$arPI = $arPI ? $arPI : array();
		foreach ($arPI as $key => $vPtres) {
			?>			
			var option = document.createElement( 'option' );
			option.setAttribute( 'value', '<?php echo $vPtres['codigo']; ?>' );
				if( pi ){
					if( pi == '<?php echo $vPtres['codigo']; ?>' ){
						option.setAttribute( 'selected', 'selected' );
						boExcluir = false;
					}
				}
					
				option.innerHTML = '<?php echo $vPtres["descricao"]; ?>';
			selectPI.appendChild( option );				
		<?php } ?>	
		
		var valor = document.createElement( "input" );
		valor.setAttribute( "type", "text" );
		valor.setAttribute( "name", "exfvalor["+emeid+"][]");
		valor.setAttribute( "id", "exfvalor[]");
		valor.setAttribute( "value", exfvalor);
		valor.setAttribute( "maxlength", "13");
		valor.setAttribute( "size", "19");
		if( exfnumempenhooriginal ){
			valor.setAttribute('readonly','readonly');
		}
		
		var pi_ptres = document.createElement( "input" );
		pi_ptres.setAttribute( "type", "hidden" );
		pi_ptres.setAttribute( "name", "pi_ptres["+emeid+"][]");
		pi_ptres.setAttribute( "id", "pi_ptres["+emeid+"]");
		pi_ptres.setAttribute( "value", ptres);
		pi_ptres.setAttribute( "maxlength", "13");
		pi_ptres.setAttribute( "size", "19");
		
		var hidEmeid = document.createElement( "input" );
		hidEmeid.setAttribute( "type", "hidden" );
		hidEmeid.setAttribute( "name", "emeid[]");
//		hidEmeid.setAttribute( "id", "emeid[]");
		hidEmeid.setAttribute( "value", emeid);
		
		if( exfvalor ){
			var exfvalor1 = exfvalor.replace(/\./gi, '');
			var exfvalor1 = exfvalor1.replace(/\,/gi, '.'); 
		}
		var valor1 = document.createElement( "input" );
		valor1.setAttribute( "type", "hidden" );
		valor1.setAttribute( "name", "exfvalor_["+emeid+"]"+ "_"+qtdL);
		valor1.setAttribute( "id", "exfvalor_["+emeid+"]"+"_"+qtdL);
		valor1.setAttribute( "value", exfvalor1);
		valor1.setAttribute( "maxlength", "13");
		valor1.setAttribute( "size", "19");
		
		var exfid = document.createElement( "input" );
		exfid.setAttribute( "type", "hidden" );
		exfid.setAttribute( "name", "exfid["+emeid+"][]");
		exfid.setAttribute( "id", "exfid["+qtdL+"]");
		exfid.setAttribute( "value", exfcod);

		// OnKeyUp
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			valor.setAttribute( "onkeyup", "this.value=mascaraglobal('[###.]###,##',this.value);" );
		}
		else{ // IE
			//valor.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "5" ); } );
		}
		// Onblur
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			valor.setAttribute( "onblur", "this.value=mascaraglobal('[###.]###,##',this.value); populaHiddenExfValor(this.value,'"+emeid+"', "+ qtdL +"); validaTotalRecurso('"+emeid+"');" );
		}
		else{ // IE
			//valor.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "5" ); } );
		}

        if( boExcluir && !exfnumempenhooriginal  ){  
			celula = linha.insertCell( 0 );
			celula.appendChild( img );
		} else {
			var img = document.createElement( "img" );
			img.setAttribute( "src", "/imagens/excluir_01.gif" );
			img.setAttribute( "title", "Excluir" );
			img.setAttribute( "alt", "Excluir" );
			celula = linha.insertCell( 0 );
			celula.appendChild( img );
		}
		
		var imgObrigatorio = document.createElement( "img" );
		imgObrigatorio.setAttribute( "src", "../imagens/obrig.gif" );
		imgObrigatorio.setAttribute( "title", "Indica campo obrigatório." );												
		
		celula = linha.insertCell( 1 );
		celula.appendChild( exfid );
		celula.appendChild( selectPTRES );
		celula.appendChild( imgObrigatorio );
		
		var imgObrigatorio1 = document.createElement( "img" );
		imgObrigatorio1.setAttribute( "src", "../imagens/obrig.gif" );
		imgObrigatorio1.setAttribute( "title", "Indica campo obrigatório." );
		
		celula = linha.insertCell( 2 );
		celula.setAttribute( 'colspan', 2 );
		celula.appendChild( selectPI );
		celula.appendChild( imgObrigatorio1 );
		//celula.appendChild( label );
		
		var imgObrigatorio2 = document.createElement( "img" );
		imgObrigatorio2.setAttribute( "src", "../imagens/obrig.gif" );
		imgObrigatorio2.setAttribute( "title", "Indica campo obrigatório." );
		
		//linha.setAttribute( "align", "left" );
		celula = linha.insertCell( 3 );
		celula.appendChild( valor );
		celula.appendChild( pi_ptres );
		celula.appendChild( imgObrigatorio2 );
		celula.appendChild( valor1 );
		celula.appendChild( hidEmeid );
	}
	function removerLinha( idLinha, emeid, exfid ){
		
		if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
			return false;
		}
		
		var tabela 	= document.getElementById( "tabelaExecucaoOrcamentaria_" + emeid );
		var linha 	= document.getElementById( idLinha );
		tabela.deleteRow( linha.rowIndex );
		if( exfid )
			excluirExecucaoFinanceira(exfid);
	}
	
	function excluirExecucaoFinanceira(exfid){
	
		var req = new Ajax.Request('emenda.php?modulo=principal/analiseGestorPTA&acao=A', {
		        method:     'post',
		        parameters: '&excluir=true&exfid='+exfid,
		        asynchronous: false,
		        onComplete: function (res){		        	
					/*if(res.responseText == "1" ){
						alert('Operação realizada com sucesso!');
					}else{
						alert('Operação não realizada!');
					}*/
					//$('erro').innerHTML = res.responseText;
		        }
		  });
	}
	
	function tramitar(documento){
		var num_documenta = '';
		if( documento == '' ){
			num_documenta = document.getElementById('ptrnumdocumenta').value;
		}else{
			num_documenta = documento;
		}
		if( num_documenta != '' ){
			num_documenta = num_documenta.split('/');
			var zero = '';
			if( num_documenta[0].length < 7 ){
				for(i=num_documenta[0].length; i<7; i++){
					zero = zero + '0';					
				}
				num_documenta[0] = zero+num_documenta[0];
			}
			num_documenta[1] = num_documenta[1].substr(0,4);
			window.open( "http://www.fnde.gov.br/pls/tramita_fnde/!tramita_fnde.tmtconrelatorio_pc?cha="+num_documenta[1] + num_documenta[0]+"&usu=03468", '_blank');
		}
	}	
	
--></script>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
<script src="../includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>

<form action="" method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="salvaranalise">
	<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid ?>">
	<?php abrePTAconsolidado($ptrid, '', 'entidade'); ?>
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center" border="0">
		<tr>
			<td colspan="3" class="subtitulocentro">Informações Gerais</td>
		</tr>
		<tr>
			<td>
				<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center" border="0">
					<tr>
			<td class="subtitulodireita" width="190px" height="30px">Nº do Processo Empenho</td>
			<td width="1000px">
				<?php 
					$boSolicitado = $db->pegaUm("select 
													case when count(exfnumsolempenho) = 0 then 'S'
													else 'N'
													end as solicitado
												 from emenda.execucaofinanceira where exfnumsolempenho is not null and exfnumsolempenho <> '' and exfstatus = 'A' and ptrid = $ptrid");
					echo campo_texto( 'ptrnumprocessoempenho', 'N', "$boSolicitado", '', 52, 50, '#####.######/####-##', '', '', '', 0, 'id="ptrnumprocessoempenho"' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="190px"  height="30px">Nº do Documento</td>
			<td>
				<?php 
					echo campo_texto( 'ptrnumdocumenta', 'N', 'S', '', 52, 50, '', '', '', '', 0, 'id="ptrnumdocumenta" onkeypress="return somenteNumeros(event);"', "this.value=mascaraglobal('#######/####-#',this.value);", '', "validaQuantidadeCaracter(this);" );
				?>
				&nbsp;&nbsp;&nbsp;<a href="#" onclick="tramitar('<?=$ptrnumdocumenta; ?>');"><img src="../imagens/consultar.gif" style="cursor:pointer;" title="Consultar histórico do documento no sistema Documenta do FNDE" border="0"></a>
			</td>
		</tr>
		<?
		$ptridPai = pegaPaiPTA( $ptrid );
		if( $_SESSION['exercicio'] != '2011' || !empty($ptridPai)  ){?>
		<tr>
			<td class="subtitulodireita" width="190px"  height="30px">Unidades que realizarão a Análise Técnica do PTA</td>
			<td>
				<?php 
					if( !$boReformulacao )
						$filtro = 'and uniid != 11';
						
					$sql = "select 
								uniid,
								uninome,
								unisigla
							from
								emenda.unidades
							where
								unistatus = 'A'
								$filtro
							order by
								uninome";
					$arrUnidades = $db->carregar($sql);
					$arrUnidades = !$arrUnidades ? array() : $arrUnidades; 
					
					$sql = "select 
								uniid
							from 
								emenda.analise 
							where 
								ptrid = $ptrid 
							and 
								anatipo = 'T'
								and anastatus = 'A'
								and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T')";
								
					$arUnidades = $db->carregar($sql);
					$arUnidades = !$arUnidades ? array() : $arUnidades;
					foreach($arUnidades as $unidade){
						$arUnidade[] = $unidade['uniid'];
					}
					$arUnidade = !$arUnidade ? array() : $arUnidade;
					
					?>
					<table>
						<tr>
					<?foreach($arrUnidades as $key => $arrUnidade){ 
						if($key == 3 || $key == $numDiv){
							$numDiv = $numDiv + 3;
							echo "</tr><tr>";
						}
						if( in_array($arrUnidade['uniid'],$arUnidade) ){
							echo '<input type="hidden" name="check[]" id="check" value="'.$arrUnidade['uniid'].'">';
						}
						?>
						<td><input <? echo in_array($arrUnidade['uniid'],$arUnidade) ? "checked='checked'" : "" ?> type="checkbox" value="<?=$arrUnidade['uniid'] ?>" name="ckb_unidade[]" title="<?=$arrUnidade['uninome'] ?>" id="unidade_<?=$arrUnidade['uniid'] ?>" /><label style="margin-right:10px" for="unidade_<?=$arrUnidade['uniid'] ?>" title="<?=$arrUnidade['uninome'] ?>" ><?=$arrUnidade['unisigla'] ?></label></td> 
					<? $cont++; }?>
						</tr>
					</table>
				
			</td>
		</tr>
		<?} ?>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td height="30px">
				<input type="button" value="Salvar" name="btnSalvar" <?php echo verificaPermissaoPerfil('analise', '', '', true); ?> id="btnSalvar" onclick="salvaAnaliseGestor();" style="cursor: pointer;">
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;">
			</td>
		</tr>
				</table>
			</td>
			<td>
				<?php
					wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid, 'resid' => '' , 'tipo' => "M" ) );
				?>
			</td>
		</tr>
	</table>
</form>
<div id="erro"></div>
<?php
/*foreach( $arDadosExef as $qtd => $v ){
	echo "<script type='text/javascript'>
				carregarRecurso( '{$v["pliptres"]}', '{$v["pliid"]}', '".number_format($v["exfvalor"], '2', ',', '.')."', '{$v['exfid']}', '{$v["exfnumempenhooriginal"]}', '".$v["eme"].'_'.$qtd."' );
				//validaTotalRecurso('".$v["eme"].'_'.$qtd."');
		  </script>";	
}*/
?>