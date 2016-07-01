<?php

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
	die("<script>alert('Não foi possível identificar a escola selecionada. Tente novamente!');
				 window.location='pdeinterativo.php?modulo=principal/principal&acao=A';</script>");
}

/*** Recupera o id do grupo de trabalho ***/
$sql = "SELECT g.grtid FROM pdeinterativo.grupotrabalho g WHERE g.grtsatatus = 'A' AND g.pdeid = ".$arrDados["pdeid"]."";
$grtid = $db->pegaUm($sql);

/*** Se ainda não houver grupo de trabalho para o diretor, cria ***/
if( !$grtid )
{
	$sql = "INSERT INTO pdeinterativo.grupotrabalho(pdeid) VALUES(".$arrDados["pdeid"].") RETURNING grtid";
	$grtid = $db->pegaUm($sql);
}

$existeMembro = $db->pegaUm("SELECT count(1) FROM pdeinterativo.pessoagruptrab WHERE grtid = ".$grtid." AND pgtstatus = 'A' AND pgtdiretor = 'f'");

if( (integer)$existeMembro < 1 )
{
	echo '<script>
			alert("É obrigatório inserir pelo menos um membro no Grupo de Trabalho, além do(a) diretor(a).");
			window.location.href = "pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=primeiropasso";
		  </script>';
	die;
}

if( $_POST['submetido'] )
{
	for($i=0; $i<count($_POST['pesid']); $i++)
	{
		if( $_POST['pgtcoordenador'][$i] )
		{
			$sql = "UPDATE pdeinterativo.pessoagruptrab SET pgtcoordenador = 'S' WHERE pesid = ".$_POST['pesid'][$i]." AND pgtstatus = 'A'";
			$db->executar($sql);
		}
		else
		{
			$sql = "UPDATE pdeinterativo.pessoagruptrab SET pgtcoordenador = 'N' WHERE pesid = ".$_POST['pesid'][$i]." AND pgtstatus = 'A'";
			$db->executar($sql);
		}
	}
	
	salvarAbaResposta("primeiros_passos_passo_2");
	
	$db->commit();
	echo '<script>
			alert("Dados gravados com sucesso.");
			window.location.href = "pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=terceiropasso";
		</script>';
	die;
}

cabecalhoPDEInterativo($arrDados);
echo monta_titulo("Primeiros Passos", "Segundo Passo");

$pesidCoordenador = $db->pegaUm("SELECT pesid FROM pdeinterativo.pessoagruptrab WHERE pgtcoordenador = 'S' AND pgtstatus = 'A' AND grtid = ".$grtid);

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function()
{
	$('input[name="opcaocoordenador"]').click(function()
	{
		if( $(this).val() == 'S' )
		{
			$('.opcaoNegativa').hide();
			$('#btSalvar').show();
			$('.opcaoPositiva').show();
		}
		else
		{
			$('#btSalvar').hide();
			$('.opcaoPositiva').hide();
			$('.opcaoNegativa').show();
		}
	});
	
	$('#btSalvar').click(function()
	{
		var flag = false;
		
		$('input[name*="pgtcoordenador"]').each(function()
		{
			if( $(this).attr('checked') )
			{
				flag = true;
			}
		});

		if(flag)
			$('#formCoordenadores').submit();
		else
			alert("É obrigatória a escolha de pelo menos um membro.");
	});

	<?php if( $pesidCoordenador ): ?>
	$('input[name="opcaocoordenador"]').each(function()
	{
		if( $(this).val() == 'S' ) $(this).attr('checked', true);
	});
	
	$('.opcaoNegativa').hide();
	$('#btSalvar').show();
	$('.opcaoPositiva').show();
	<?php endif; ?>

	$('input[name*="pgtcoordenador"]').click(function()
	{
		$('input[name*="pgtcoordenador"]').each(function()
		{
			$(this).attr('checked', '');
		});

		$(this).attr('checked', 'checked');
	});
});

function abreJanela()
{
	var janela = window.open('pdeinterativo.php?modulo=principal/popSegundoPasso&acao=A', 'popSegundoPasso', 'width=500,height=320,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	janela.focus();
}

</script>

<form id="formCoordenadores" method="post" action="">
<input type="hidden" name="submetido" value="1" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="10" cellPadding="10" align="center">
	<tr>
		<td>
			<img border="0" src="/imagens/bussola.png" />
		</td>
		<td>
		<font style="color:#1719fd;">
		Depois de criar o Grupo de Trabalho (GT) é necessário definir quem será o(a) Coordenador(a) do plano. Recomenda-se que não seja o(a) diretor(a), pois este geralmente já possui um grande número de atribuições. A principal responsabilidade do(a) coordenador(a) é conduzir e animar o processo de planejamento, atuando como líder junto à equipe. 
		<br /><br />
		Para saber mais sobre o perfil do(a) Coordenador(a) do plano, <a href="#" onclick="abreJanela();">clique aqui</a>.
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
			A escola definiu quem será o(a) Coordenador(a) do plano?
			<input type="radio" name="opcaocoordenador" value="S" />Sim
			<input type="radio" name="opcaocoordenador" value="N" />Não
		</td>
	</tr>
	
	<tr class="opcaoPositiva" style="background-color:#dcdcdc;text-align:center;display:none;">
		<td colspan="2" style="font-weight:bold;">Coordenador(a) do Plano</td>
	</tr>
	<tr class="opcaoPositiva" style="display:none;">
		<td align="right" class="SubTituloDireita" width="350px" style="color:#1719fd;">
			Orientações
		</td>
		<td style="color:#1719fd;">
			<br />
			Se a escola já definiu o(a) Coordenador(a) do plano, indique-o(a) clicando no botão ao lado do nome. Em seguida, clique em "Salvar e continuar". É obrigatório designar apenas um(a) Coordenador(a).
			<br /><br />
			Caso deseje alterar alguma informação sobre o(a) Coordenador(a), é necessário alterar estes dados no Passo 1 (Membros do GT). Neste caso, volte para a tela anterior, altere os dados clicando no botão "Editar" ( <img src="/imagens/check_p.gif" border="0" style="vertical-align:middle;" /> ) e, em seguida clique em "Salvar".
			<br /><br />
		</td>
	</tr>
	<tr class="opcaoPositiva" style="display:none;">
		<td align="right" class="SubTituloDireita" width="350px"></td>
		<td>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="5" align="center" style="border-top:1px solid black;" id="tbCoordenadores">
				<thead>
					<tr>
						<th colspan="6">Relação dos Membros do GT - Escolha o(a) Coordenador(a)</th>
					</tr>
					<tr>
						<th>&nbsp;</th>
						<th>Nome</th>
						<th>CPF</th>
						<th>Perfil</th>
						<th>(DDD) + Telefone</th>
						<th>Email</th>
					</tr>
				</thead>
				<tbody id="corpoTabelaCoordenadores">
				<?php
				if( $grtid )
				{
					$sql = "SELECT
								p.pesid,
								CASE WHEN p.pesnome is not null THEN p.pesnome ELSE u.usunome END AS pesnome,
								p.usucpf,
								CASE WHEN fg.fgtdesc is not null THEN fg.fgtdesc ELSE '-' END AS fgtdesc,
								CASE WHEN dp.dpetelefone is not null THEN dp.dpetelefone ELSE u.usufoneddd || u.usufonenum END AS dpetelefone,
								CASE WHEN dp.dpeemail is not null THEN dp.dpeemail ELSE u.usuemail END AS dpeemail,
								g.pgtdiretor
							FROM
								pdeinterativo.pessoagruptrab g
							INNER JOIN
								pdeinterativo.pessoa p ON p.pesid = g.pesid
							LEFT JOIN
								pdeinterativo.detalhepessoa dp ON dp.pesid = p.pesid
							LEFT JOIN
								pdeinterativo.funcaogt fg ON fg.fgtid = dp.fgtid
							LEFT JOIN
								seguranca.usuario u ON u.usucpf = p.usucpf
							WHERE
								g.grtid = ".$grtid."
								AND g.pgtstatus = 'A'";
					$dadosMembros = $db->carregar($sql);
					
					if( $dadosMembros )
					{
						$cont = 0;
						foreach($dadosMembros as $membro)
						{
							$cpf		= substr($membro['usucpf'], 0, 3).'.'.substr($membro['usucpf'], 3, 3).'.'.substr($membro['usucpf'], 6, 3).'-'.substr($membro['usucpf'], 9);
							
							$membro['dpetelefone'] = str_replace("-", "", $membro['dpetelefone']);
							
							$ddd 		= substr($membro['dpetelefone'], 0, 2);
							$telefone 	= substr($membro['dpetelefone'], 2);
							$telefone	= substr($telefone, 0, 4).'-'.substr($telefone, 4);
							
							$checked = ( $membro['pesid'] == $pesidCoordenador ) ? 'checked="checked"' : '';
							
							if( $membro['pgtdiretor'] == 't') $membro['fgtdesc'] = 'Diretor(a)';
							
							echo '<tr>
									<td align="center">
										<input type="hidden" value="'.$membro['pesid'].'" name="pesid['.$cont.']">
										<input type="radio" name="pgtcoordenador['.$cont.']" '.$checked.' />
									</td>
									<td>'.$membro['pesnome'].'</td>
									<td>'.$cpf.'</td>
									<td>'.$membro['fgtdesc'].'</td>
									<td>('.$ddd.') '.$telefone.'</td>
									<td>'.$membro['dpeemail'].'</td>
								 </tr>';
							
							$cont++;
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
			A escola só poderá continuar o seu Plano após a designação do(a) Coordenador(a). Leia abaixo como designar o(a) Coordenador(a).
			<br /><br />
		</td>
	</tr>
	<tr class="opcaoNegativa" style="display:none;">
		<td align="right" class="SubTituloDireita" width="350px">
			Perfil do(a) Coordenador(a)
		</td>
		<td>
		<br>
		O(A) Coordenador(a) do Plano de Desenvolvimento da Escola deve ser, necessariamente, um membro do Grupo de Trabalho, escolhido pelo próprio GT com a anuência do(a) diretor(a). Um(a) candidato(a) natural ao cargo de coordenador(a) é o(a) coordenador(a) pedagógico(a) da escola. Ele(a) tem por função principal animar o processo de elaboração, orientar o grupo e coordenar as ações que devem ser tomadas para a elaboração, a execução, o monitoramento e a avaliação do planejamento. 
		<br /><br />
		Uma característica essencial do(a) coordenador(a) é que ele(a) conheça bem a escola, entenda os principais indicadores educacionais e saiba interpretá-los, ajudando o GT a identificar os principais desafios e definir as ações necessárias para enfrentá-los."
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
			<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=primeiropasso'" >
			<input type="button" value="Salvar e continuar" id="btSalvar" style="display:none;" />
			<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=terceiropasso'" >
		</td>
	</tr>
</table>

</form>