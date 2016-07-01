<?php
ini_set('post_max_size', '300M');
ini_set('upload_max_filesize', '300M');
ini_set("memory_limit", "2048M");
set_time_limit(0);


if( $_REQUEST['arqid_download'] )
{
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$campos = array('tpdid' => TPDID_GRUPO_TRABALHO);
	$file = new FilesSimec("anexo", $campos, "pdeinterativo");
	ob_clean();
	$file->getDownloadArquivo($_REQUEST['arqid_download']);
}

if( !$_SESSION['pdeinterativo_vars']['usucpfdiretor'] )
{
	echo '<script>
			alert("Você deve ter perfil de Diretor(a) para acessar esta página.");
			window.location.href = "pdeinterativo.php?modulo=principal/principal&acao=A";
		  </script>';
	exit;
}

$arrDados = recuperaDadosEscolaPorCPFDiretor($_SESSION['pdeinterativo_vars']['usucpfdiretor']);

if(!$arrDados["pdeid"]) {
	echo '<script>
			alert("Você deve ter perfil de Diretor(a) para acessar esta página.");
			window.location.href = "pdeinterativo.php?modulo=principal/principal&acao=A";
		  </script>';
	exit;
}

cabecalhoPDEInterativo($arrDados);
echo monta_titulo("Primeiros Passos", "Primeiro Passo");

/*** Recupera o id do grupo de trabalho ***/
$sql = "SELECT g.grtid FROM pdeinterativo.grupotrabalho g WHERE g.grtsatatus = 'A' AND g.pdeid = ".$arrDados["pdeid"]."";
$grtid = $db->pegaUm($sql);

/*** Se ainda não houver grupo de trabalho para o diretor, cria ***/
if( !$grtid )
{
	$sql = "INSERT INTO pdeinterativo.grupotrabalho(pdeid) VALUES(".$arrDados["pdeid"].") RETURNING grtid";
	$grtid = $db->pegaUm($sql);
	$db->commit();
}

/*** Verifica se o CPF do diretor na sessão está no grupo de trabalho ***/
if( $db->pegaUm("SELECT count(pgt.*) FROM pdeinterativo.pessoagruptrab pgt INNER JOIN pdeinterativo.pessoa p ON p.pesid = pgt.pesid AND p.usucpf = '".$_SESSION['pdeinterativo_vars']['usucpfdiretor']."' WHERE pgt.grtid = ".$grtid." AND pgt.pgtstatus = 'A'") < 1 )
{
	/*** Recupera o pesid do diretor ***/
	$pesid = $db->pegaUm("SELECT pesid FROM pdeinterativo.pessoa WHERE usucpf = '".$_SESSION['pdeinterativo_vars']['usucpfdiretor']."' AND pesstatus = 'A'");
	
	$sql = "SELECT pgtid from pdeinterativo.pessoagruptrab where pesid=".$pesid." AND grtid=".$grtid."";
	$pgtid = $db->pegaUm($sql);
	
	if($pgtid) {
		$sql = "DELETE FROM pdeinterativo.pessoagruptrab WHERE pgtid='".$pgtid."'";
		$db->executar($sql);
		$db->commit();
	}
	
	/*** Se já existir diretor, altera para o novo diretor ***/
	if( $db->pegaUm("SELECT count(1) FROM pdeinterativo.pessoagruptrab WHERE grtid = ".$grtid." AND pgtstatus = 'A' AND pgtdiretor = 't'") > 0 ) {
		$sql = "UPDATE pdeinterativo.pessoagruptrab SET pesid = ".$pesid." WHERE grtid = ".$grtid." AND pgtdiretor = 't' AND pgtstatus = 'A'";
	} else {
		$sql = "INSERT INTO pdeinterativo.pessoagruptrab(pesid,grtid,pgtdiretor,pdeid) VALUES(".$pesid.", ".$grtid.", 't','".$arrDados["pdeid"]."')";
	}
	$db->executar($sql);
	$db->commit();
	
	salvarAbaResposta("primeiros_passos_passo_1");
	
}

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function()
{
	$('input[name="opcaogrupo"]').click(function()
	{
		var recarrega = false;
		
		if( $(this).val() == 'N' )
		{
			if( existeDadosGT(<?=$grtid?>) )
			{
				if( confirm("Esta ação irá apagar os dados previamente cadastrados, associados a este Grupo de Trabalho.\nDeseja continuar?") )
				{
					apagaDadosGT(<?=$grtid?>);
					recarrega = true;
				}
				else
				{
					$('input[name="opcaogrupo"]').each(function()
					{
						if( $(this).val() == 'S' ) $(this).attr('checked', true);
					});
					return;
				}
			}
		}
		
		$.ajax({
			   type: "post",
			   url: "pdeinterativo.php?modulo=principal/primeirospassos&acao=A",
			   async: false,
			   data: "ajaxOpcaoGrupo=1&grtid=<?=$grtid?>&opcao="+$(this).val(),
			   success: function(msg)
			   {
			   	 if( msg == 'ok' )
			   	 {
					if( recarrega )
					{
						window.location.href = 'pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=primeiropasso';
						return;
					}
			   	 }
			   	 else
			   	 {
			   		alert('Ocorreu um erro ao gravar a opção.');
			   		return;
			   	 }
			   }
		});
		
		if( $(this).val() == 'S' )
		{
			$('.opcaoNegativa').hide();
			$('#btContinuar').show();
			$('.opcaoPositiva').show();
		}
		else
		{
			$('#btContinuar').hide();
			$('.opcaoPositiva').hide();
			$('.opcaoNegativa').show();
		}
	});

	$('#btContinuar').click(function()
	{
		window.location.href = 'pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=segundopasso';
	});

	$('#btIncluirMembro').click(function()
	{
		var janela = window.open('pdeinterativo.php?modulo=principal/popIncluirMembroGT&acao=A&grtid=<?=$grtid?>', 'popIncluirMembroGT', 'width=500,height=260,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
		janela.focus();
	});

	<?php $grtopcao = $db->pegaUm("SELECT grtopcao FROM pdeinterativo.grupotrabalho WHERE grtid = ".$grtid); ?>
	
	<?php if( $grtopcao == 'S' ): ?>
	$('input[name="opcaogrupo"]').each(function()
	{
		if( $(this).val() == 'S' ) $(this).attr('checked', true);
	});
	
	$('.opcaoNegativa').hide();
	$('#btContinuar').show();
	$('.opcaoPositiva').show();
	<?php endif; ?>
	
	<?php if( $grtopcao == 'N' ): ?>
	$('input[name="opcaogrupo"]').each(function()
	{
		if( $(this).val() == 'N' ) $(this).attr('checked', true);
	});
	
	$('#btContinuar').hide();
	$('.opcaoPositiva').hide();
	$('.opcaoNegativa').show();
	<?php endif; ?>
});

function apagaDadosGT(grtid)
{
	$.ajax({
		   type: "post",
		   url: "pdeinterativo.php?modulo=principal/primeirospassos&acao=A",
		   async: false,
		   data: "ajaxApagaDadosGT=1&grtid="+grtid,
		   success: function(msg)
		   {
				if( msg == 'ok' )
			   	{
					// ...
			   	}
			   	else
			   	{
			   		alert('Ocorreu um erro ao excluir os dados.');
			   		return;
			   	}
		   }
	});
}

function existeDadosGT(grtid)
{
	var ret;
	
	$.ajax({
		   type: "post",
		   url: "pdeinterativo.php?modulo=principal/primeirospassos&acao=A",
		   async: false,
		   dataType: 'json',
		   data: "ajaxExisteDados=1&grtid="+grtid,
		   success: function(msg)
		   {
		     ret = msg.retorno;
		   }
	});

	return ret;
}

function abreJanela()
{
	var janela = window.open('pdeinterativo.php?modulo=principal/popPrimeiroPasso&acao=A', 'popPrimeiroPasso', 'width=500,height=350,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	janela.focus();
}

function alteraMembro(index, pesid)
{
	var janela = window.open('pdeinterativo.php?modulo=principal/popIncluirMembroGT&acao=A&index='+index+'&pesid='+pesid+'&grtid=<?=$grtid?>', 'popAlteraMembroGT', 'width=500,height=260,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	janela.focus();
}

function excluiMembro(index, pesid)
{
	if( confirm("Deseja excluir o membro do GT?") )
	{
		$.ajax({
			   type: "post",
			   url: "pdeinterativo.php?modulo=principal/primeirospassos&acao=A",
			   async: false,
			   data: "ajaxExcluirMembro=1&pesid="+pesid+"&grtid=<?=$grtid?>",
			   success: function(msg)
			   {
			   	 if( msg == 'ok' )
			   	 {
					var tabela = document.getElementById('tbMembros');
					tabela.deleteRow(index);
					alert('Membro excluído com sucesso.');
			   	 }
			   	 else
			   	 {
			   		alert('Ocorreu um erro ao excluir o membro.');
			   		return;
			   	 }
			   }
		});
	}
}

function sleep(milliseconds)
{
	var start = new Date().getTime();
	
  	for (var i = 0; i < 1e7; i++)
  	{
    	if( (new Date().getTime() - start) > milliseconds )
        {
      		break;
    	}
  	}
}

function salvarArquivo()
{
	var arq = document.getElementById('arquivo');
	
	if( arq.value == '' )
	{
		alert("O campo 'Arquivo' deve ser preenchido.");
		arq.focus();
		return;
	}
	
	document.getElementById('file_upload_form').target = 'upload_target';
	document.getElementById('file_upload_form').submit();

	alert('Arquivo incluído com sucesso.');
	window.location.href = 'pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=primeiropasso';
}

function excluiDocumento(index, arqid)
{
	if( confirm("Deseja realmente excluir o documento anexado?") )
	{
		$.ajax({
			   type: "post",
			   url: "pdeinterativo.php?modulo=principal/primeirospassos&acao=A",
			   async: false,
			   data: "ajaxApagaArquivo=1&arqid="+arqid,
			   success: function(msg)
			   {
				if( msg == 'ok' )
			   	 {
					var tabela = document.getElementById('tbMembrosArquivo');
					tabela.deleteRow(index);
					alert('Documento excluído com sucesso.');
			   	 }
			   	 else
			   	 {
			   		alert('Ocorreu um erro ao excluir o documento.');
			   		return;
			   	 }
			   }
		});
	}
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="10" cellPadding="10" align="center">
	<tr>
		<td>
			<img border="0" src="/imagens/bussola.png" />
		</td>
		<td>
		<font style="color:#1719fd;">
		A preparação do plano começa com a definição da equipe responsável pela sua elaboração, formada por pessoas que se dispuserem a participar do planejamento. Sugere-se que o Conselho Escolar seja convidado a realizar esta atividade, podendo ser acrescido de outros membros que representem a direção da escola, os professores, funcionários e alunos dos diversos segmentos. Caso a escola não tenha Conselho Escolar, recomenda-se que seja constituído um Grupo de Trabalho formado por 5 (cinco) a 10 (dez) pessoas e que este seja o responsável pela elaboração do plano estratégico da escola. 
				<br /><br />
		Para saber quais são os perfis e atribuições do Grupo de Trabalho, <a href="#" onclick="abreJanela();">clique aqui</a>..
		</font>
		</td>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="5" align="center" style="border-top:1px solid black;">
	<tr>
		<td align="right" class="SubTituloDireita" width="350px">
			Pergunta
		</td>
		<td>
			A escola definiu o Grupo de Trabalho que será responsável pela elaboração do plano?
			<input type="radio" name="opcaogrupo" value="S" />Sim
			<input type="radio" name="opcaogrupo" value="N" />Não
		</td>
	</tr>
	<tr class="opcaoPositiva" style="background-color:#dcdcdc;text-align:center;display:none;">
		<td colspan="2" style="font-weight:bold;">Grupo de Trabalho</td>
	</tr>
	<tr class="opcaoPositiva" style="display:none;">
		<td align="right" class="SubTituloDireita" width="350px" style="color:#1719fd;">
			Orientações
		</td>
		<td style="color:#1719fd;">
			<br />
			Se a escola já definiu o Grupo de Trabalho, vamos cadastrar os seus membros. Para inserir os membros do GT, clique no botão "Incluir Membro". Em seguida, será apresentado um formulário. Preencha os campos indicando o nome, o perfil que representa e os contatos (telefone e e-mail) de cada membro do GT. Em seguida, clique em "Salvar". É obrigatório incluir pelo menos um nome.
			<br /><br />
			Caso deseje inserir novos nomes, repita a operação. Caso deseje excluir algum nome, basta clicar no símbolo "Excluir" ( <img src="/imagens/exclui_p.gif" border="0" style="vertical-align:middle;" /> ). O sistema perguntará se você deseja mesmo excluir aquele nome. Em caso afirmativo, clique "SIM". Caso deseje cancelar a operação, clique em "Cancelar". Caso deseje alterar alguma informação de uma ou mais pessoas, clique no símbolo "Editar" ( <img src="/imagens/check_p.gif" border="0" style="vertical-align:middle;" /> ), modifique os dados e clique em "Salvar".
			<br /><br />
		</td>
	</tr>
	<tr class="opcaoPositiva" style="display:none;">
		<td align="right" class="SubTituloDireita" width="350px" style="color:#1719fd;"></td>
		<td style="color:#1719fd;">
			<span style="margin-left:20px;"><input type="button" value="Incluir Membro" id="btIncluirMembro" /></span>
			<br /><br />
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="5" align="center" style="border-top:1px solid black;" id="tbMembros">
				<thead>
					<tr>
						<th colspan="6">Membros do Grupo de Trabalho</th>
					</tr>
					<tr>
						<th>Editar/Excluir</th>
						<th>Nome</th>
						<th>CPF</th>
						<th>Perfil</th>
						<th>(DDD) + Telefone</th>
						<th>Email</th>
					</tr>
				</thead>
				<tbody id="corpoTabela">
					<?php 
					$sql = "SELECT
								u.usunome as pesnome,
								p.usucpf,
								CASE WHEN fg.fgtdesc is not null THEN fg.fgtdesc ELSE '-' END AS fgtdesc,
								u.usufoneddd || u.usufonenum AS dpetelefone,
								u.usuemail AS dpeemail
							FROM
								pdeinterativo.pessoa p
							LEFT JOIN
								pdeinterativo.detalhepessoa dp ON dp.pesid = p.pesid
							LEFT JOIN
								pdeinterativo.funcaogt fg ON fg.fgtid = dp.fgtid
							LEFT JOIN
								seguranca.usuario u ON u.usucpf = p.usucpf
							WHERE
								p.usucpf = '".$_SESSION['pdeinterativo_vars']['usucpfdiretor']."'
							and
								pesstatus = 'A'";
					
					$dadosDiretor = $db->carregar($sql);
					
					if( $dadosDiretor )
					{
						$cpf		= substr($dadosDiretor[0]['usucpf'], 0, 3).'.'.substr($dadosDiretor[0]['usucpf'], 3, 3).'.'.substr($dadosDiretor[0]['usucpf'], 6, 3).'-'.substr($dadosDiretor[0]['usucpf'], 9);
						
						$dadosDiretor[0]['dpetelefone'] = str_replace("-", "", $dadosDiretor[0]['dpetelefone']);
						$ddd 		= substr($dadosDiretor[0]['dpetelefone'], 0, 2);
						$telefone 	= substr($dadosDiretor[0]['dpetelefone'], 2);
						$telefone	= substr($telefone, 0, 4).'-'.substr($telefone, 4);
					?>
					<tr>
						<td align="center">
							<img border="0" src="/imagens/check_p_01.gif" title="Altera as informações do membro do GT" style="cursor:pointer;">&nbsp;<img border="0" src="/imagens/exclui_p2.gif" style="cursor:pointer;" title="Exclui o membro do GT">
						</td>
						<td>
							<?=$dadosDiretor[0]['pesnome']?>
						</td>
						<td>
							<?=$cpf?>
						</td>
						<td>
							Diretor(a)
						</td>
						<td>
							(<?=$ddd?>) <?=$telefone?>
						</td>
						<td>
							<?=$dadosDiretor[0]['dpeemail']?>
						</td>
					</tr>
					<?php
					}
					 
					if( $grtid )
					{
						$sql = "SELECT
									p.pesid,
									p.pesnome,
									p.usucpf,
									fg.fgtid,
									fg.fgtdesc,
									dp.dpetelefone,
									dp.dpeemail
								FROM
									pdeinterativo.pessoagruptrab g
								INNER JOIN
									pdeinterativo.pessoa p ON p.pesid = g.pesid
								INNER JOIN
									pdeinterativo.detalhepessoa dp ON dp.pesid = p.pesid
								INNER JOIN
									pdeinterativo.funcaogt fg ON fg.fgtid = dp.fgtid
								WHERE
									g.grtid = ".$grtid."
									AND g.pgtstatus = 'A'
									AND g.pgtdiretor = 'f'";
						$dadosMembros = $db->carregar($sql);
						
						if( $dadosMembros )
						{
							foreach($dadosMembros as $membro)
							{
								$cpf		= substr($membro['usucpf'], 0, 3).'.'.substr($membro['usucpf'], 3, 3).'.'.substr($membro['usucpf'], 6, 3).'-'.substr($membro['usucpf'], 9);
								$ddd 		= substr($membro['dpetelefone'], 0, 2);
								$telefone 	= substr($membro['dpetelefone'], 2);
								$telefone	= substr($telefone, 0, 4).'-'.substr($telefone, 4);
								
								echo '<tr>
										<td align="center"><img border="0" onclick="alteraMembro(this.parentNode.parentNode.rowIndex, '.$membro['pesid'].');" src="/imagens/check_p.gif" title="Altera as informações do membro do GT" style="cursor:pointer;">&nbsp;<img border="0" onclick="excluiMembro(this.parentNode.parentNode.rowIndex, '.$membro['pesid'].');" src="/imagens/exclui_p.gif" style="cursor:pointer;" title="Exclui o membro do GT"></td>
										<td>'.iconv("UTF-8", "ISO-8859-1", $membro['pesnome']).'</td>
										<td>'.$cpf.'</td>
										<td>'.$membro['fgtdesc'].'</td>
										<td>('.$ddd.') '.$telefone.'</td>
										<td>'.$membro['dpeemail'].'</td>
									 </tr>';
							}
						}
					}
					?>
				</tbody>
			</table>
		</td>
	</tr>
	<tr class="opcaoPositiva" style="background-color:#dcdcdc;text-align:center;display:none;">
		<td colspan="2" style="font-weight:bold;">Ata de Criação do Grupo de Trabalho</td>
	</tr>
	<tr class="opcaoPositiva" style="display:none;">
		<td align="right" class="SubTituloDireita" width="350px" style="color:#1719fd;">
			Orientações
		</td>
		<td style="color:#1719fd;">
			<br />
			Agora insira a ata de criação do Grupo de Trabalho. A ata é o documento que contem o registro da reunião que definiu os membros do grupo, devidamente datada e assinada pelos presentes.
			<br /><br />
			Para anexar a ata, o arquivo precisa estar salvo no computador. Clique em "Selecionar arquivo", procure o documento no computador e clique em "Inserir".
			<br /><br />
			Caso deseje inserir outros documentos relativos à criação do GT, repita a operação. Caso deseje excluir algum documento, basta clicar no símbolo "Excluir" ( <img src="/imagens/exclui_p.gif" border="0" style="vertical-align:middle;" /> ). O sistema perguntará se você deseja mesmo excluir aquele documento. Em caso afirmativo, clique "SIM". Caso deseje cancelar a operação, clique em "Cancelar".
			<br /><br />
			<font color="red">Atualize o navegador, caso não consiga anexar o documento.</font>
		</td>
	</tr>
	<tr class="opcaoPositiva" style="display:none;">
		<td align="right" class="SubTituloDireita" width="350px">
			Arquivo
		</td>
		<td>
		<form id="file_upload_form" method="post" enctype="multipart/form-data" action="pdeinterativo.php?modulo=principal/upload_arquivo_gt&acao=A">
			<input type="file" id="arquivo" name="arquivo" onchange="salvarArquivo();" /> *Obs: O tamanho máximo de arquivo aceito é de 200MB.
			<input type="hidden" id="arq_grtid" name="arq_grtid" value="<?=$grtid?>" />
		</form>
		<iframe id="upload_target" name="upload_target" src="" style="width:0;height:0;border:0px solid #fff;"></iframe>
		</td>
	</tr>
	<tr class="opcaoPositiva" style="display:none;">
		<td align="right" class="SubTituloDireita" width="350px"></td>
		<td>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="5" align="center" style="border-top:1px solid black;" id="tbMembrosArquivo">
				<thead>
					<tr>
						<th colspan="3">Documentos Anexados</th>
					</tr>
					<tr>
						<th>Excluir</th>
						<th>Nome</th>
						<th>Data de Inclusão</th>
					</tr>
				</thead>
				<tbody id="corpoTabelaArquivo">
				<?php
				
				if( $grtid )
				{
					$sql = "SELECT
								ar.arqid,
								ar.arqnome || '.' || ar.arqextensao as arqnome,
								to_char(ar.arqdata, 'DD/MM/YYYY') as arqdata
							FROM
								pdeinterativo.gruptrabanexo g
							INNER JOIN
								pdeinterativo.anexo a ON a.anxid = g.anxid
													 AND a.anxstatus = 'A'
													 AND a.tpdid = ".TPDID_GRUPO_TRABALHO."
							INNER JOIN
								public.arquivo ar ON ar.arqid = a.arqid
							WHERE
								g.grtid = ".$grtid."
								AND g.gtastatus = 'A'";
					$dadosArquivos = $db->carregar($sql);
					
					if( $dadosArquivos )
					{
						foreach($dadosArquivos as $arquivo)
						{
							echo '<tr>
									<td align="center"><img title="Exclui o documento anexado" style="cursor:pointer;" src="/imagens/exclui_p.gif" border="0" onclick="excluiDocumento(this.parentNode.parentNode.rowIndex, '.$arquivo['arqid'].');" /></td>
									<td><a href="pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=primeiropasso&arqid_download='.$arquivo['arqid'].'">'.$arquivo['arqnome'].'</a></td>
									<td align="center">'.$arquivo['arqdata'].'</td>
								 </tr>';
						}
					}
				}
				
				?>
				</tbody>
			</table>
		</td>
	</tr>
	<tr class="opcaoNegativa" style="color:red;display:none;">
		<td align="right" class="SubTituloDireita" width="350px">
			Aviso
		</td>
		<td>
			<br />
			A escola só poderá elaborar o seu Plano após a criação do Grupo de Trabalho e anexação do documento comprobatório. Leia abaixo como constituir o Grupo de Trabalho. 
			<br /><br />
		</td>
	</tr>
	<tr class="opcaoNegativa" style="display:none;">
		<td align="right" class="SubTituloDireita" width="350px">
			Como constituir o GT
		</td>
		<td>
			<br />
			O Grupo de Trabalho é uma equipe composta, no mínimo, pela liderança formal da escola (diretor(a), vice-diretor(a), coordenador(a) pedagógico(a), orientador(a), secretário(a)), por um (a)representante dos docentes, um dos pais ou responsáveis e um representante dos estudantes, de todos os turnos e níveis.
			<br /><br />
			Esse grupo é liderado pelo(a) diretor(a), que deverá divulgar o convite junto à comunidade escolar. O número de pessoas dependerá da estrutura da escola e da disposição de membro em participar voluntariamente da elaboração e acompanhamento do plano. Não recomendamos mais de 10 (dez) membros, sob o risco de inviabilizar ou retardar a elaboração do plano.
			<br /><br />
			São responsabilidades do Grupo de Trabalho: convocar reuniões, elaborar o plano, encaminhar e acompanhar a análise do plano junto à Secretaria de Educação, acompanhar a implementação e execução do PDE Escola e promover avaliações contínuas do plano.
			<br /><br />
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="5" align="center" style="border-top:1px solid black;">
	<tr>
		<td align="right" class="SubTituloDireita" width="350px">
			O que deseja fazer agora?
		</td>
		<td>
			<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=orientacoes'" >
			<input type="button" value="Salvar e Continuar" id="btContinuar" style="display:none;" />
			<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=segundopasso'" >
		</td>
	</tr>
</table>