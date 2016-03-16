<?php

if( $_REQUEST['sbaid'] ){
	
	function salvarBeneficiarios()
	{
		$oBen = new SubacaoBeneficiario();
		$oBen->deletaBeneficiarioPorSubacao($_GET['sbaid'],$_GET['ano']);
		
		$n = 0;
		
		if( is_array($_POST['benid']) ){
			foreach($_POST['benid'] as $chave => $benid){
				if($benid && ( $_POST['sabqtdurbano'][$chave] || $_POST['sabqtdrural'][$chave] )){
					$oBen = new SubacaoBeneficiario();
					$oBen->sbaid 		= $_GET['sbaid'];
					$oBen->benid 		= $benid;
					$oBen->sabqtdurbano = $_POST['sabqtdurbano'][$chave] ? str_replace(".","",$_POST['sabqtdurbano'][$chave]) : 0;
					$oBen->sabqtdrural  = $_POST['sabqtdrural'][$chave] ? str_replace(".","",$_POST['sabqtdrural'][$chave]) : 0;
					$oBen->sabano 		= $_GET['ano'];
					$oBen->salvar();
					$oBen->commit();
					$n++;
				}
			}
		}
		
		if($n == 0){
			echo "<script>
					alert('Não houve modificações!');
					window.close();
				</script>";
		}else{
			echo "<script>
					alert('Operação realizada com sucesso!');
					parent.window.opener.location.href = 'par.php?modulo=principal/subacao&acao=A&sbaid={$_GET['sbaid']}&anoatual={$_GET['ano']}';
					window.close();
				</script>";	
		}
		
		exit;
	}
	
	if($_POST['requisicao']){
		$_POST['requisicao']();
	}
	
	function comboBeneficiario($nome = "benid[]", $value = null)
	{
		global $db;
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		$sql = "select
					b.benid as codigo,
					b.bendsc as descricao
				from
					par.beneficiario b
				INNER JOIN
					par.subacao s ON s.sbaid = b.sbaid AND s.sbastatus = 'A'
				order by
					b.bendsc";
		return $db->monta_combo($nome,$sql,"S","Selecione...","","","","","N","","",$value);
	}
	
	function texto_qtde($nome = "sabqtdrural[]", $value = null)
	{
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		echo campo_texto($nome,"N","S","","10","20","[.###]","","right","","","","",$value);
	}
	
	$sbaid = $_REQUEST['sbaid'];
	if(!$sbaid){
		echo "<script>alert('Subação não encontrada!');window.close();</script>";
	}
	
	if($_POST['requisicaoAjax']){
		header('content-type: text/html; charset=ISO-8859-1');
		$_POST['requisicaoAjax']();
		die;
	}
	
	/** Carrega informações de localização e dados da subação  **/
	$sql = "SELECT  s.sbaid,
					s.sbadsc,
					s.ptsid,
					s.foaid,
					d.dimid,
					d.dimcod,
					d.dimdsc,
					ar.areid,
					ar.arecod,
					ar.aredsc,
					i.indcod,
					i.indid,
					i.inddsc,
					a.acidsc,
					p.ptoid,
					s.sbaestrategiaimplementacao,
					s.undid,
					s.prgid,
					s.frmid,
					pps.ppscronograma
			FROM par.subacao 	 	 s
			INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus = 'A'
			INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus = 'A'
			INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A' 
			INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
			INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
			INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
			LEFT JOIN par.propostasubacao pps ON pps.ppsid = s.ppsid
			WHERE s.sbastatus = 'A' AND s.sbaid = $sbaid";
	
	$subacao = $db->pegaLinha($sql);
	
	?>
	<html>
		<head>
		    <meta http-equiv="Cache-Control" content="no-cache">
		    <meta http-equiv="Pragma" content="no-cache">
		    <meta http-equiv="Connection" content="Keep-Alive">
		    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
		    <title>PAR 2010 - Plano de Metas - Subação</title>
		
		    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		    <link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>
		    <script type="text/javascript" src="../includes/funcoes.js"></script>
		    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		</head>
		<body>
			<script type="text/javascript">
				function salvarBeneficiarios()
				{
					var erro = 0;
					$("[class~=obrigatorio]").each(function() { 
						if(!this.value || this.value == "Selecione..."){
							erro = 1;
							alert('Favor preencher todos os campos obrigatórios!');
							this.focus();
							return false;
						}
					});
					if(erro == 0){
						$("#form_subacao").submit();
					}
				}
				function novoBeneficiario()
				{
					var num = $("#tbl_beneficiario tr").length;
					$("#tbl_beneficiario").append('<tr id="tr_' + num + '" ><td align="center" ><img class="middle link" title="Excluir Beneficiário" onclick="removerLinha(\'' + num + '\')"  src="../imagens/excluir.gif" /></td><td id="benid_' + num + '" align="center" ></td><td  id="sabqtdrural_' + num + '" align="center" ></td><td  id="sabqtdurbano_' + num + '" align="center" ></td></tr>');
					JqueryAjax("requisicaoAjax=texto_qtde&nome=sabqtdrural[" + num + "]","sabqtdrural_" + num);
					JqueryAjax("requisicaoAjax=texto_qtde&nome=sabqtdurbano[" + num + "]","sabqtdurbano_" + num);
					JqueryAjax("requisicaoAjax=comboBeneficiario&nome=benid[" + num + "]","benid_" + num);
				}
				function removerLinha(num)
				{
					$("#tr_" + num).remove();
				}
				function JqueryAjax(params,destino)
				{
					$.ajax({
						   type: "POST",
						   url: window.location,
						   data: params,
						   success: function(msg){
								$("#" + destino).html(msg);
						   }
					 });
				}
			</script>
			<form name="form_subacao" id="form_subacao" method="post" action="" >
				<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" />
				<input type="hidden" name="requisicao" value="salvarBeneficiarios" />
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr style="background-color: #cccccc;">
						<td align="center" colspan="2"><strong>Beneficiários</strong></td>
					</tr>
					<tr>
						<td align="left" colspan="2">
							<?php $oBen = new SubacaoBeneficiario();?>
							<?php $arrDados = $oBen->lista(null,array("sbaid = '{$_GET['sbaid']}'", "sabano = '{$_GET['ano']}'")) ?>
							<table id="tbl_beneficiario" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
								<tr class="SubTituloTabela bold" >
									<td class="bold" >Ação</td>
									<td class="bold" >Beneficiário</td>
									<td class="bold" >Zona Rural</td>
									<td class="bold" >Zona Urbana</td>
								</tr>
								<?php if($arrDados): ?>
									<?php foreach($arrDados as $chave => $dado): ?>
										<tr id="tr_<?php echo $chave ?>" >
											<td align="center" ><img class="middle link" title="Excluir Beneficiário" onclick="removerLinha('<?php echo $chave ?>')"  src="../imagens/excluir.gif" /></td>
											<td align="center" ><?php comboBeneficiario("benid[$chave]", $dado['benid']) ?></td>
											<td align="center" ><?php texto_qtde("sabqtdrural[$chave]", $dado['sabqtdrural'] ? number_format($dado['sabqtdrural'],0,2,'.') : "" ) ?></td>
											<td align="center" ><?php texto_qtde("sabqtdurbano[$chave]", $dado['sabqtdurbano'] ? number_format($dado['sabqtdurbano'],0,2,'.') : "" ) ?></td>
										</tr>	
									<?php endforeach; ?>
								<?php endif; ?>
								<?php $num = $arrDados ? count($arrDados) : "1" ?>
								<tr id="tr_<?php echo $num ?>" >
									<td align="center" ><img class="middle link" title="Excluir Beneficiário" onclick="removerLinha('<?php echo $num ?>')"  src="../imagens/excluir.gif" /></td>
									<td align="center" ><?php comboBeneficiario("benid[$num]") ?></td>
									<td align="center" ><?php texto_qtde("sabqtdrural[$num]") ?></td>
									<td align="center" ><?php texto_qtde("sabqtdurbano[$num]") ?></td>
								</tr>
							</table>
						</td>
					</tr>
					<tr style="background-color: #cccccc;">
						<td align="left" colspan="2">
							<img class="middle link" onclick="novoBeneficiario()"  src="../imagens/gif_inclui.gif" /> <a href="javascript:novoBeneficiario()">Adicionar Beneficiário</a>
							<span style="margin-left:100px;color:red" class="bold" >*Obs: Para confirmar as alterações clique no botão "Salvar".</span>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita"></td>
						<td class="SubTituloEsquerda" >
							<input type="button" value="Salvar" name="btn_salvar" onclick="salvarBeneficiarios()" />
							<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
						</td>
					</tr>
				</table>
			</form>
		</body>
	</html>
<?php } else { //VEIO DO MENU!! 

	if( $_REQUEST['adicionaBenid'] ){
		global $db;
		
		$ano = $_REQUEST['ano'];
		
		$sql = "SELECT
					'<center><img class=\"middle link\" title=\"Excluir Escola\" src=\"../imagens/excluir.gif\" onclick=\"excluirBenid(this,\'' || benid || '\')\"  /></center>' as acao,
					bendsc as descricao,
					'<input type=\"text\" id=\"rural[{$ano}][' || benid || ']\" name=\"rural[{$ano}][' || benid || ']\" value=\"\">' as rural,
					'<input type=\"text\" id=\"urbano[{$ano}][' || benid || ']\" name=\"urbano[{$ano}][' || benid || ']\" value=\"\">' as urbana
				FROM
					par.beneficiario
				WHERE
					benid IN ({$_REQUEST['benidselecionados']}) 
				ORDER BY
					bendsc";
		
		$cabecalho = array("Ação","Descrição","Zona Rural","Zona Urbana");
		return $db->monta_lista_simples($sql,$cabecalho,100,5,"N","100%");

	}
	
	if($_POST['requisicao']){
		$_POST['requisicao']();
	}
	
	function comboBeneficiario($nome = "benid[]", $value = null)
	{
		global $db;
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		$sql = "select
					benid as codigo,
					bendsc as descricao
				from
					par.beneficiario 
				order by
					bendsc";
		return $db->monta_combo($nome,$sql,"S","Selecione...","","","","","N","","",$value);
	}
	
	function texto_qtde($nome = "sabqtdrural[]", $value = null)
	{
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		echo campo_texto($nome,"N","S","","10","20","[.###]","","right","","","","",$value);
	}
	
	if($_POST['requisicaoAjax']){
		header('content-type: text/html; charset=ISO-8859-1');
		$_POST['requisicaoAjax']();
		die;
	}
	
?>
	<html>
		<head>
		    <meta http-equiv="Cache-Control" content="no-cache">
		    <meta http-equiv="Pragma" content="no-cache">
		    <meta http-equiv="Connection" content="Keep-Alive">
		    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
		    <title>PAR 2010 - Plano de Metas - Subação</title>
		
		    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		    <link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>
		    <script type="text/javascript" src="../includes/funcoes.js"></script>
		    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		</head>
		<body>
			<script type="text/javascript">
				function salvarBeneficiariosMenu()
				{
					var erro = 0;
					$("[class~=obrigatorio]").each(function() { 
						if(!this.value || this.value == "Selecione..."){
							erro = 1;
							alert('Favor preencher todos os campos obrigatórios!');
							this.focus();
							return false;
						}
					});
					
					ano = jQuery( '#ano' ).val();
					
					jQuery( '#beneficiarios' ).val( '0' );

					$("[name^='benid[']").each(function(){
						beneficiarios = jQuery( '#beneficiarios' ).val();
						jQuery( '#beneficiarios' ).val( beneficiarios + ',' + $(this).val() );
					});

					benidselecionados = jQuery( '#beneficiarios' ).val();

					if(erro == 0){
						jQuery.ajax({
							  type		: 'post',
							  url		: window.location,
							  data		: 'adicionaBenid=1&benidselecionados=' + benidselecionados,
							  success	: function(res) {
						  			window.opener.jQuery( '#beneficiarios_' + ano ).html( res );
							  }
						});
					}
				}
				function novoBeneficiario()
				{
					var num = $("#tbl_beneficiario tr").length;
					$("#tbl_beneficiario").append('<tr id="tr_' + num + '" ><td align="center" ><img class="middle link" title="Excluir Beneficiário" onclick="removerLinha(\'' + num + '\')"  src="../imagens/excluir.gif" /></td><td id="benid_' + num + '" align="center" ></td></tr>');
					JqueryAjax("requisicaoAjax=texto_qtde&nome=sabqtdrural[" + num + "]","sabqtdrural_" + num);
					JqueryAjax("requisicaoAjax=texto_qtde&nome=sabqtdurbano[" + num + "]","sabqtdurbano_" + num);
					JqueryAjax("requisicaoAjax=comboBeneficiario&nome=benid[" + num + "]","benid_" + num);
				}
				function removerLinha(num)
				{
					$("#tr_" + num).remove();
				}
				function JqueryAjax(params,destino)
				{
					$.ajax({
						   type: "POST",
						   url: window.location,
						   data: params,
						   success: function(msg){
								$("#" + destino).html(msg);
						   }
					 });
				}
			</script>
			<form name="form_subacao" id="form_subacao" method="post" action="" >
				<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" />
				<input type="hidden" name="ano" id="ano" value="<?php echo $_REQUEST['ano'] ?>" />
				<input type="hidden" name="beneficiarios" id="beneficiarios" value="" />
				<input type="hidden" name="requisicao" value="salvarBeneficiarios" />
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr style="background-color: #cccccc;">
						<td align="center" colspan="2"><strong>Beneficiários</strong></td>
					</tr>
					<tr>
						<td align="left" colspan="2">
							<?php $oBen = new SubacaoBeneficiario();?>
							<?php //$arrDados = $oBen->lista(null,array("sbaid = '{$_GET['sbaid']}'", "sabano = '{$_GET['ano']}'")) ?>
							<table id="tbl_beneficiario" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
								<tr class="SubTituloTabela bold" >
									<td class="bold" >Ação</td>
									<td class="bold" >Beneficiário</td>
								</tr>
								<?php if($arrDados): ?>
									<?php foreach($arrDados as $chave => $dado): ?>
										<tr id="tr_<?php echo $chave ?>" >
											<td align="center" ><img class="middle link" title="Excluir Beneficiário" onclick="removerLinha('<?php echo $chave ?>')"  src="../imagens/excluir.gif" /></td>
											<td align="center" ><?php comboBeneficiario("benid[$chave]", $dado['benid']) ?></td>
											<td align="center" ><?php texto_qtde("sabqtdrural[$chave]", $dado['sabqtdrural'] ? number_format($dado['sabqtdrural'],0,2,'.') : "" ) ?></td>
											<td align="center" ><?php texto_qtde("sabqtdurbano[$chave]", $dado['sabqtdurbano'] ? number_format($dado['sabqtdurbano'],0,2,'.') : "" ) ?></td>
										</tr>	
									<?php endforeach; ?>
								<?php endif; ?>
								<?php $num = $arrDados ? count($arrDados) : "1" ?>
								<tr id="tr_<?php echo $num ?>" >
									<td align="center" ><img class="middle link" title="Excluir Beneficiário" onclick="removerLinha('<?php echo $num ?>')"  src="../imagens/excluir.gif" /></td>
									<td align="center" ><?php comboBeneficiario("benid[$num]") ?></td>
								</tr>
							</table>
						</td>
					</tr>
					<tr style="background-color: #cccccc;">
						<td align="left" colspan="2">
							<img class="middle link" onclick="novoBeneficiario()"  src="../imagens/gif_inclui.gif" /> <a href="javascript:novoBeneficiario()">Adicionar Beneficiário</a>
							<span style="margin-left:100px;color:red" class="bold" >*Obs: Para confirmar as alterações clique no botão "Salvar".</span>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita"></td>
						<td class="SubTituloEsquerda" >
							<input type="button" value="Salvar" name="btn_salvar" onclick="salvarBeneficiariosMenu()" />
						</td>
					</tr>
				</table>
			</form>
		</body>
	</html>
<?php } ?>