<?php
if (!$_REQUEST["AJAX"]){
    include APPRAIZ . 'includes/cabecalho.inc';
    include APPRAIZ . 'includes/Agrupador.php';

    echo "<br/>";
}

include APPRAIZ . '/profeinep/modulos/controller.inc';
$processoProfeinep = new ProcessoProfeinep();
if($_REQUEST["pcjid"] != ''){
	$_SESSION["pcjid"] = $_REQUEST["pcjid"];
}
if($_SESSION["pcjid"]){
	$processoProfeinep->Dados($_SESSION["pcjid"]);
}
$db->cria_aba($abacod_tela,$url,$parametros);

include APPRAIZ . '/profeinep/modulos/principal/informacoes.inc';

?>
<script src="./js/profeinep.js"></script>
<script src="../includes/calendario.js"></script>
<script type="text/javascript">

function mudarPropriedade(tipoElement,targetInput){
	var input = $_(targetInput);
	input.value="";
	if(tipoElement.value == 0){
		new classCpf($_(targetInput));
	}else{
		new classCnpj($_(targetInput));
	}
	
}
novoProcesso = function(url){
		window.location = url;
		return; 
}
RemoveLinha = function(index)
{
	table = window.document.getElementById("tabela_interessado");
	table.deleteRow(index);
}
//
</script>
<html>
<body>
<form name="formulario" id="pesquisar" method="POST" action="?modulo=principal/dados_processo&acao=I">
<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
<input type="hidden" name="pcjid" value="<? echo $processoProfeinep->__get("pcjid");  ?>" />
<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><b>Cadastro da Processo</b></td>
		</tr>

		<tr>
			<td class="SubTituloDireita">Número do Processo SIDOC:</td>
			<td>
			<?
			$pcjnumerosidoc = $processoProfeinep->__get("pcjnumerosidoc"); 
			echo campo_texto( 'pcjnumerosidoc', 'N', 'S', '',35, 30, '', '', 'left', 'pcjnumerosidoc');
			?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Processo na PROFE/INEP:</td>
			<td>
			<?
			$pcjnumero = $processoProfeinep->__get("pcjnumero");
			$sqlSituacao = "SELECT tppid as codigo, tppnome as descricao  from profeinep.tipoprocessoprofeinep";
			$db->monta_combo("pcjtipoprocesso", $sqlSituacao, "S", "", '', '', '', '150', 'N','pcaid');			
			echo " Número:".campo_texto( 'pcjnumero', 'N', 'S', '',20, 20, '', '', 'left', 'pcjnumero');
			?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">Situação na PROFE/INEP:</td>
			<td>
			<?
			$pcjnumero = $processoProfeinep->__get("pcjstatus");
			$sqlSituacao = "SELECT stcid as codigo, stcnome as descricao FROM profeinep.situacaoprocessoprofeinep";
			$db->monta_combo("pcjstatus", $sqlSituacao, "S", "", '', '', '', '150', 'N','pcjstatus');			
			?>
			</td>
		</tr>
				
		<tr>
			<td class="SubTituloDireita">Tribunal:</td>
			<td>
			<?
			$pcjtribunal = $processoProfeinep->__get("pcjtribunal");
			echo campo_texto( 'pcjtribunal', 'N', 'S', '', 30, 30, '', '', 'left', 'pcjtribunal');
			?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Vara:</td>
			<td>
			<? 
			$pcjvara = $processoProfeinep->__get("pcjvara");
			echo campo_texto( 'pcjvara', 'N', 'S', '', 30, 30, '', '', 'left', 'pcjvara');
			?>
			</td>
		</tr>		
		
		<tr>
			<td class="SubTituloDireita">Data Entrada:</td>
			<td>
			<?
			$pcjdtentrada = $processoProfeinep->__get("pcjdtentrada"); 
			echo campo_data( 'pcjdtentrada', 'N', 'S', '', 'S' );
			?>
			</td>
		</tr>	

		<tr>
			<td class="SubTituloDireita">Procedência:</td>
			<td>
			<? 
			$pcjprocedencia = $processoProfeinep->__get("pcjprocedencia");
			$sqlProcedencia = "SELECT ungcod as codigo, ungdsc as descricao FROM public.unidadegestora where unicod = 26101";
			$db->monta_combo("pcjprocedencia", $sqlProcedencia, "S", "", '', '', '', '300', 'N','pcjprocedencia');
			?>
			</td>			
		</tr>
				
		<tr>
			<td class="SubTituloDireita">Assunto:</td>
			<td>
			<? 
			$pcjassunto = $processoProfeinep->__get("pcjassunto");
			$sqlAssunto = "SELECT pcaid as codigo, pcanome as descricao  from profeinep.processoassunto";
			$db->monta_combo("pcjassunto", $sqlAssunto, "S", "", '', '', '', '300', 'N','pcjassunto');
			?>
			</td>			
		</tr>
		
		<tr>
			<td class="SubTituloDireita" >Informações:</td>
			<td>
			<? 	
			$pcjinformacoes = $processoProfeinep->__get("pcjinformacoes");
			echo campo_textarea( 'pcjinformacoes', 'N', $somenteLeitura, '', '70', '4', '500'); ?>
	
			</td>			
		</tr>
		<tr>		
		<td  bgcolor="#CCCCCC" colspan="2" align="center"><b>Interessados no Processo</b>
		<table id="tabela_interessado" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<thead>
				<tr id="cabecalho">
				<td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
				<td width="60%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nome</strong></td>
				<td width="15%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo Pessoa</strong></td>
				</tr>
			<?php
			if($_SESSION["pcjid"]){
				$sqlInterressados = " SELECT * FROM profeinep.processointeressados as i, entidade.entidade as e where i.entid = e.entid AND i.pcjid = ".$_SESSION["pcjid"]."
				";
				$interessado = $db->executar($sqlInterressados);				
				while($info = pg_fetch_assoc($interessado)) {
					if(strlen($info[entnumcpfcnpj]) == 11){
						$tpPessoa = "Física";
					}else{
						$tpPessoa = "Jurídica";
					}
					echo '<tr>
					<td>
					<input type="hidden" value="'.$info[entid].'" id="entid_'.$info[entid].'" name="entid[]"/>
					<img src="/imagens/alterar.gif" style="cursor: pointer" border="0" title="Editar" onclick="atualizaInteressado(\'linha_' . $info[entid]. '\');"/>&nbsp&nbsp&nbsp<img src="/imagens/excluir.gif" style="cursor: pointer"  border="0" title="Excluir" onclick="RemoveLinha(window.document.getElementById(\\\'linha_'.$info[entid].'\\\').rowIndex);"/></td>
					<td>' . $info[entnome]. '</td>
					<td>' . $tpPessoa. '</td>
					</tr>';
				}
			}
			?>				
			</thead>
		</table>

		</td>
		</tr>
		<tr>
			<td>
				<a href="#" onclick="inserirInteressado();"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Interessados"> Inserir Interessados</a>
			</td>
		</tr>		
				
	<tr><td colspan="2"></br></td>
	</tr>
	<tr>
		<td  bgcolor="#CCCCCC" colspan="2" align="center"><b>Palavras Chaves</b>
		<table id="tabela_palavrachave" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<thead>
				<tr id="cabecalho">
				<td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
				<td width="60%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nome</strong></td>
				</tr>
				<tr>
				<td>
				<img src="/imagens/alterar.gif" style="cursor: pointer" border="0" title="Editar" onclick="atualizaInteressado(\\\'linha_' . $entidade->getPrimaryKey(). '\\\');"/>&nbsp&nbsp&nbsp<img src="/imagens/excluir.gif" style="cursor: pointer"  border="0" title="Excluir" onclick="RemoveLinha(window.document.getElementById(\\\'linha_'.$entidade->getPrimaryKey().'\\\').rowIndex);"/>
				</td>
				<td> 
				<?php
					echo campo_texto( 'pecexpressao[]', 'N', 'S', '', 30, 30, '', '', 'left', '', 0, '');
				?>
				</td>
				</tr>
			</thead>
		</table>

		</td>
		</tr>
			<tr>
			<td  bgcolor="#CCCCCC" colspan="2" align="center">
					<input type="submit" value="Salvar" style="cursor: pointer"> 
					<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
			</td>
		</tr>			
</table>
</form>		
