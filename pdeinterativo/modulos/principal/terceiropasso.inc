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

/*** Recupera o id do grupo de trabalho ***/
$sql = "SELECT g.grtid FROM pdeinterativo.grupotrabalho g WHERE g.grtsatatus = 'A' AND g.pdeid = ".$arrDados["pdeid"]."";
$grtid = $db->pegaUm($sql);

/*** Se ainda não houver grupo de trabalho para o diretor, cria ***/
if( !$grtid )
{
	$sql = "INSERT INTO pdeinterativo.grupotrabalho(pdeid) VALUES(".$arrDados["pdeid"].") RETURNING grtid";
	$grtid = $db->pegaUm($sql);
}

$existeCoordenador = $db->pegaUm("SELECT count(1) FROM pdeinterativo.pessoagruptrab WHERE grtid = ".$grtid." AND pgtstatus = 'A' AND pgtcoordenador = 'S'");

if( (integer)$existeCoordenador < 1 )
{
	echo '<script>
			alert("É obrigatório determinar o coordenador do Grupo de Trabalho");
			window.location.href = "pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=segundopasso";
		  </script>';
	die;
}

cabecalhoPDEInterativo($arrDados);
echo monta_titulo("Primeiros Passos", "Terceiro Passo");

$grtconcluido = $db->pegaUm("SELECT grtconcluido FROM pdeinterativo.grupotrabalho WHERE grtid = ".$grtid);

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function()
{
	<?php if( !$grtconcluido || $grtconcluido == 'f' ): ?>
	$('#btVisualizarDados').click(function()
	{
		$.ajax({
			   type: "post",
			   url: "pdeinterativo.php?modulo=principal/primeirospassos&acao=A",
			   async: false,
			   data: "ajaxConcluiPrimeirosPassos=1&grtid=<?=$grtid?>",
			   success: function(msg)
			   {
					if( msg == 'ok' )
				   	{
						window.location.href = window.location.href;
				   	}
				   	else
				   	{
				   		alert('Ocorreu um erro ao visualizar os dados.');
				   		return;
				   	}
			   }
		});
	});
	<?php endif; ?>
});

function abreJanela()
{
	var janela = window.open('pdeinterativo.php?modulo=principal/popTerceiroPasso&acao=A&grtid=<?=$grtid?>', 'popTerceiroPasso', 'width=500,height=180,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	janela.focus();
}

</script>

<form id="formComite" method="post" action="">
<input type="hidden" name="submetido" value="1" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="10" cellPadding="10" align="center">
	<tr>
		<td>
			<img border="0" src="/imagens/bussola.png" />
		</td>
		<td>
		<font style="color:#1719fd;">
		Já estamos no último passo! Agora, basta conhecer o Comitê de Análise e Aprovação do plano (se a escola participa do PDE Escola) ou do Plano de Ações Articuladas (PAR). Este comitê tem como principal responsabilidade analisar, revisar, aprovar, encaminhar para o MEC e acompanhar a execução do plano da escola. Este grupo também é responsável pelo gerenciamento do cadastro dos(as) diretores(as).
		<br /><br />
		<?php if( !$grtconcluido || $grtconcluido == 'f' ): ?>
		Clique no botão abaixo e conheça as principais atribuições e os membros do Comitê de Análise e Aprovação da sua secretaria.
		<br /><br />
		<input type="button" value="CLIQUE AQUI para visualizar os dados" id="btVisualizarDados" />
		<?php endif; ?>
		</font>
		</td>
	</tr>
</table>
<?php 
if( $grtconcluido == 't' )
{
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="5" align="center" style="border-top:1px solid black;">
	<tr>
		<td align="right" class="SubTituloDireita" width="350px" style="color:#1719fd;">
			Perfil e atribuições do comitê
		</td>
		<td style="color:#1719fd;">
			<br />
			As principais atribuições dos técnicos das secretárias responsáveis pelo planejamento estratégico das escolas são: i) prestar assistência técnica na elaboração e execução dos programas federais e; ii) analisar e emitir parecer para o MEC acerca dos planos de todas as escolas de sua rede (estadual ou municipal) com base em critérios técnicos, pedagógicos e financeiros. Por isso, é imprescindível que os técnicos das secretarias visitem as escolas e conheçam bem os programas federais.
			<br /><br />
			<b>Lembre-se:</b> Para o MEC, os técnicos que acessam este módulo do SIMEC representam a palavra da Secretaria de Educação, de modo que, ao encaminhar o plano para o Ministério, entende-se que o mesmo foi efetivamente aprovado pela Secretária. Por esta razão, recomenda-se que esta equipe seja designada formalmente pelo(a) dirigente de Educação, por meio de um decreto, portaria ou qualquer outro instrumento que caracterize a criação deste grupo.
			<br /><br />
			No link <b>“Principal” > “Documentos Comitês”</b> é possível fazer o download do documento enviado pela Secretaria de Educação para o MEC.
			<br /><br />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" width="350px"></td>
		<td>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="5" align="center" style="border-top:1px solid black;" id="tbComite">
				<thead>
					<tr>
						<th colspan="10">Membros do Comitê</th>
					</tr>
					<tr>
						<th>Nome</th>
						<th>Gênero</th>
						<th>UF</th>
						<th>Município</th>
						<th>Tipo de Órgão/Insituição</th>
						<th>Telefone</th>
						<th>Email</th>
						<th>Função/Cargo</th>
					</tr>
				</thead>
				<tbody id="corpoTabelaComite">
				<?php
				
				$arrPde = $db->pegaLinha("SELECT pd.pdigeridapde, pd.pdiesfera FROM pdeinterativo.pdinterativo pd WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
				$pdiesfera = $arrPde['pdiesfera'];
				$pdigeridapde = $arrPde['pdigeridapde'];
				
				$where	= " usuchaveativacao=true and ususis.suscod='A'";
				
				if($pdigeridapde=="t") {
					if( $pdiesfera == "Estadual" ) {
						$perfil = PDEINT_PERFIL_COMITE_ESTADUAL;
						$where	= " usuchaveativacao=true and ususis.suscod='A' and ur.estuf in(SELECT estuf FROM pdeinterativo.pdinterativo WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."')";
					}
					if( $pdiesfera == "Municipal" ) {
						$perfil = PDEINT_PERFIL_COMITE_MUNICIPAL;
						$where	= " usuchaveativacao=true and ususis.suscod='A' and ur.estuf is null and ur.muncod in(SELECT muncod FROM pdeinterativo.pdinterativo WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."')";
					}
				}
				
				if($pdigeridapde=="f") {
					
					if( $pdiesfera == "Estadual" ) {
						$perfil = PDEINT_PERFIL_COMITE_PAR_ESTADUAL;
						$where	= " usuchaveativacao=true and ususis.suscod='A' and ur.estuf in(SELECT estuf FROM pdeinterativo.pdinterativo WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."')";
					}
					if( $pdiesfera == "Municipal" ) {
						$perfil = PDEINT_PERFIL_COMITE_PAR_MUNICIPAL;
						$where	= " usuchaveativacao=true and ususis.suscod='A' and ur.estuf is null and ur.muncod in(SELECT muncod FROM pdeinterativo.pdinterativo WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."')";
					}
				
				}
				
				if(!$perfil) die("<script>alert('Problemas na identificação da Esfera.');window.location='pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=terceiropasso';</script>");
					
				$sql = "SELECT DISTINCT
							u.usunome as nome,
							CASE WHEN u.ususexo = 'M' THEN 'Masculino' ELSE 'Feminino' END as genero,
							CASE WHEN e.estdescricao IS NULL THEN ee.estdescricao ELSE e.estdescricao END as uf,
							m.mundescricao as municipio,
							CASE WHEN tpo.tpodsc is null THEN '-' ELSE tpo.tpodsc END as tipo_orgao,
							'(' || u.usufoneddd || ') ' || u.usufonenum as telefone,
							u.usuemail as email,
							pfldsc as funcao
						FROM
							seguranca.usuario u
						INNER JOIN
							seguranca.perfilusuario pu ON pu.usucpf = u.usucpf
													  AND pu.pflcod in (".$perfil.")
						INNER JOIN
							seguranca.perfil pe ON pe.pflcod = pu.pflcod

						INNER JOIN 
							pdeinterativo.usuarioresponsabilidade ur ON ur.pflcod='".$perfil."' AND ur.usucpf = u.usucpf and ur.rpustatus='A' 
						INNER JOIN 
							seguranca.usuario_sistema ususis ON u.usucpf = ususis.usucpf AND ususis.susstatus = 'A' AND ususis.sisid = ".SISID_PDE_INTERATIVO."
						LEFT JOIN
							territorios.municipio m ON m.muncod = ur.muncod
						LEFT JOIN
							territorios.estado e ON e.estuf = m.estuf
						LEFT JOIN
							territorios.estado ee ON ee.estuf = ur.estuf
						LEFT JOIN
							public.tipoorgao tpo ON tpo.tpocod = u.tpocod
						WHERE
							$where
						ORDER BY
							u.usunome ASC";
							
				$dadosComite = $db->carregar($sql);
					
				
				if( $dadosComite )
				{
					foreach($dadosComite as $membro)
					{
						$cpf = substr($membro['cpf'], 0, 3).'.'.substr($membro['cpf'], 3, 3).'.'.substr($membro['cpf'], 6, 3).'-'.substr($membro['cpf'], 9);
						
						echo '<tr>
								<td>'.$membro['nome'].'</td>
								<td>'.$membro['genero'].'</td>
								<td>'.$membro['uf'].'</td>
								<td>'.$membro['municipio'].'</td>
								<td>'.$membro['tipo_orgao'].'</td>
								<td>'.$membro['telefone'].'</td>
								<td>'.$membro['email'].'</td>
								<td>'.$membro['funcao'].'</td>
							 </tr>';
						
						$cont++;
					}
				}
				?>
				</tbody>
			</table>
		</td>
	</tr>
</table>
<?php } ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="5" align="center" style="border-top:1px solid black;">
	<tr>
		<td align="right" class="SubTituloDireita" width="350px">
			O que deseja fazer agora?
		</td>
		<td>
			<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=segundopasso'" >
			<input type="button" value="Continuar" id="btContinuar" onclick="abreJanela();" />
			<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/diagnostico&acao=A'" >
		</td>
	</tr>
</table>

</form>