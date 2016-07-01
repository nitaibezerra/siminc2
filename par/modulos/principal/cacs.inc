<?php

/*
protected $arAtributos = array(
		'co_mun_ibge_conselho' => null,
		'esf_adm_conselho' => null,
		'sit_mandato' => null,
		'uf_conselho' => null,
		'mun_conselho' => null,
		'email_conselho' => null,
		'cpf_conselheiro' => null,
		'no_conselheiro' => null,
		'email_conselheiro' => null,
		'sit_conselheiro' => null,
		'ds_segmento' => null,
		'tp_membro' => null,
		'ds_funcao' => null
);

public function __construct()
{
	$GLOBALS['nome_bd'] = '';
	$GLOBALS['servidor_bd'] = '';
	$GLOBALS['porta_bd'] = '';
	$GLOBALS['usuario_db'] = '';
	$GLOBALS['senha_bd'] = '';
}

public function listarConselheiros($muncod)
{
	$sql = "SELECT *
	FROM {$this->stNomeTabela}
	WHERE co_mun_ibge_conselho = '{$muncod}'";

	return $this->carregar($sql);
}
*/

if( !$_SESSION['par']['inuid'] ){
	echo "
		<script>
			alert('Sessão expirada.');
			location.href='par.php?modulo=inicio&acao=C';
 		</script>";
	die();
}

require_once APPRAIZ . 'includes/cabecalho.inc';

$inuid = $_SESSION['par']['inuid'];

print "<br />";

print carregaAbaDadosUnidade("/par/par.php?modulo=principal/cacs&acao=A");

monta_titulo( "Dados da unidade", "CACS" );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

//Pega os perfis do usuário
$arrPerfil = pegaPerfilGeral();

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore">
				<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
			</a>
		</td>
		<td style="color: blue; font-size: 12px; text-align:right">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A">Voltar à Árvore |</a>
				<label style="color:black; text-align:center">
					<b>Dados da Unidade</b>
				</label>
			<a href="par.php?modulo=principal/listaObrasParUnidade&acao=A">| Lista de Obras </a>
			<a href="par.php?modulo=principal/questoesPontuais&acao=A">| Questões Pontuais</a>
		</td>
	</tr>
</table>

<?

$cacs = adapterConnection::coorporativo()->carregar("SELECT replace(to_char(cpf_conselheiro::numeric, '000:000:000-00'), ':', '.') as cpf_conselheiro, 
															no_conselheiro, 
															email_conselheiro, 
														    sit_conselheiro, 
															ds_segmento, 
															tp_membro,
															ds_funcao 
													 FROM integracao.cacs 
													 WHERE co_mun_ibge_conselho = '".$_SESSION['par']['muncod']."' ORDER BY no_conselheiro");

$cabecalho = array("CPF","Nome","Email","Situação","Atuação","Vinculo","Cargo");
$db->monta_lista_array($cacs,$cabecalho,50,10,'N','center');

?>


	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr bgcolor="#C0C0C0" align="center">
			<td>
				<input type="button" class="botao" name="anterior" value='Anterior' onclick="window.location='par.php?modulo=principal/orgaoEducacao&acao=A&funid=15'" > 
				<input type="button" class="botao" name="proximo" value='Próximo' onclick="window.location='par.php?modulo=principal/equipeLocal&acao=A'" >
			</td>
		</tr>
	</table>
