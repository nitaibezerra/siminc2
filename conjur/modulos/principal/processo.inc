<?php
if (!$_REQUEST["AJAX"]){
    include APPRAIZ . 'includes/cabecalho.inc';
    include APPRAIZ . 'includes/Agrupador.php';

    echo "<br/>";
}

include APPRAIZ . '/conjur/modulos/controller.inc';
$processoConjur = new ProcessoConjur(); 
if($_SESSION["pcjid"]){
	$processoConjur->Dados($_SESSION["pcjid"]);
	$sqlInterressados = " SELECT * FROM conjur.processointeressados as i, entidade.entidade as e where i.entid = e.entid AND i.pcjid = ".$_SESSION["pcjid"]."
				";
	$interessado = $db->executar($sqlInterressados);

}
$db->cria_aba($abacod_tela,$url,$parametros);

include APPRAIZ . '/conjur/modulos/principal/informacoes.inc';

?>

<html>
<body>
<form name="formulario" id="pesquisar" method="POST" action="">
<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
		<input type="hidden" name="pesquisa" value="1">
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><b>Argumentos da Pesquisa</b></td>
		</tr>

		<tr>
			<td class="SubTituloDireita">Número do Processo SIDOC:</td>
			<td>
			<? 
			$pcjnumerosidoc = $_POST["pcjnumerosidoc"];
			echo campo_texto( 'pcjnumerosidoc', 'N', 'S', '',30, 30, '', '', 'left', '', 0, '');
			?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Número:</td>
			<td>
			<? 
			$pcjtipoprocesso = $_POST["pcjtipoprocesso"];
			$sqlSituacao = "SELECT tppid as codigo, tppnome as descricao  from conjur.tipoprocessoconjur";
			$db->monta_combo("pcjtipoprocesso", $sqlSituacao, "S", "Selecione...", '', '', '', '150', 'N','pcjtipoprocesso');			
			$pcjnumero = $_POST["pcjnumero"];
			echo campo_texto( 'pcjnumero', 'N', 'S', '',20, 20, '', '', 'left', 'pcjnumero', 0, '');
			?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Tribunal:</td>
			<td>
			<? 
			$pcjtribunal = $_POST["pcjtribunal"];
			echo campo_texto( 'pcjtribunal', 'N', 'S', '', 60, 60, '', '', 'left', 'pcjtribunal', 0, '');
			?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Vara:</td>
			<td>
			<? 
			$pcjvara = $_POST["pcjvara"];
			echo campo_texto( 'pcjvara', 'N', 'S', '', 60, 60, '', '', 'left', 'pcjvara', 0, '');
			?>
			</td>
		</tr>		
		
		<tr>
			<td class="SubTituloDireita">Data Entrada:</td>
			<td>
			<? 
			$pcjdtentrada = $_POST["pcjdtentrada"];
			echo campo_data( 'pcjdtentrada', 'N', 'S', '', 'S' );
			echo " Fim: ";
			echo campo_data( 'pfim', 'N', 'S', '', 'S' );
			?>
			</td>
		</tr>	
		
		<tr>
			<td class="SubTituloDireita">Advogados:</td>
			<td>
			<? 
			$obrdesc = $_POST["obrdesc"];
			?>
			<input type="button" name="pesquisar_advogado" value="Advogado(s)" style="cursor: pointer;" onclick="procurarAdvogados();">
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">CPF/CNPJ:</td>
			<td>
			
			<? 
			$obrdesc = $_POST["obrdesc"];
			echo "<input type='radio' name='stoid' value='0' onchange='mudarPropriedade(this,\"cpfcnpj\")'/> CPF";
			echo "<input type='radio' name='stoid' value='1'  onchange='mudarPropriedade(this,\"cpfcnpj\")'/> CNPJ ";
			?>
			<input name="cpfcnpj" id="cpfcnpj" size="20" value="" class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align: left; width: 18ex;" title="" type="text">
			
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Tramitados-MEC:</td>
			<td>
			<? 
			$obrdesc = $_POST["obrdesc"];
			?>
			<input type="button" name="pesquisarTramitadosMec" value="Pesquisar" style="cursor: pointer;" onclick="pesquisarTramitadosMec();">
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Tramitados-Advogados:</td>
			<td>
			<? 
			$obrdesc = $_POST["obrdesc"];
			?>
			<input type="button" name="pesquisarTramitadosAdv" value="Pesquisar" style="cursor: pointer;" onclick="pesquisarTramitadosAdv();">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Expressão chave:</td>
			<td>
			<? 
			$obrdesc = $_POST["obrdesc"];
			echo campo_texto( 'obrdesc', 'N', 'S', '', 60, 60, '', '', 'left', '', 0, '');
			?>
			</td>			
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação Conjur:</td>
			<td>
			<? 
			$pcjstatus= $_POST["pcjstatus"];
			$sqlSituacao = "SELECT stcid as codigo, stcnome as descricao  from conjur.situacaoprocessoconjur";
			$db->monta_combo("pcjstatus", $sqlSituacao, "S", "Selecione...", '', '', '', '300', 'N','pcjstatus');
			?>
			</td>			
		</tr>
		<tr>
			<td class="SubTituloDireita">Assunto:</td>
			<td>
			<? 
			$pcjassunto = $_POST["pcjassunto"];
			$sqlAssunto = "SELECT pcaid as codigo, pcanome as descricao  from conjur.processoassunto";
			$db->monta_combo("pcjassunto", $sqlAssunto, "S", "Selecione...", '', '', '', '300', 'N','pcjassunto');
			?>
			</td>			
		</tr>
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2" align="center">
			<input type="button" name="pesquisarTramitadosAdv" value="Visualizar" style="cursor: pointer;" onclick="visualizarProcessos();">  
			<input type="button" name="pesquisarTramitadosAdv" value="Ver Todos" style="cursor: pointer;" onclick="visualizarProcessos(T);">
			</td>
		</tr>
		
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2" align="center">
			<?php if($_REQUEST["pesquisa"]) {?>
		<table id="resultaoPesquisa" width="100%" align="center" border="2" cellspacing="2" cellpadding="2" class="listagem">
			<thead>
				<tr id="cabecalho">
				<td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
				<td width="30%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Número Processo SIDOC</strong></td>
				<td width="15%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Número do Processo</strong></td>
				<td width="50%" valign="top"  align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Interessados</strong></td>
				</tr>
			<?php

			$return = $db->executar($sqlProcesso);
			
			while($info = pg_fetch_assoc($return)) {
				echo '<tr>
				<td  valign="top">
				<img src="/imagens/alterar.gif" style="cursor: pointer" border="0" title="Editar" onclick="abrirProcesso(\'?modulo=principal/dados_processo&acao=A&pcjid=' . $info[pcjid]. '\');"/></td>
				<td  valign="top">' . $info[pcjnumerosidoc]. '</td>
				<td  valign="top">' . $info[pcjnumero]. '</td>';
				$sqlInterressado = 	"SELECT * FROM conjur.processointeressados as p, entidade.entidade as e
								where p.entid = e.entid AND p.pcjid = ".$info[pcjid];
				echo '<td  valign="top">';
				$returnI = $db->executar($sqlInterressado);
				while($infoInt = pg_fetch_assoc($returnI)) {
					
					echo $infoInt[entnome]."</br>";
				}
				echo '</td>';
				echo '</tr>';
			}
		
			?>				
			</thead>
		</table>
		<?php } ?>
			</td>
		</tr>					
</table>
</form>		