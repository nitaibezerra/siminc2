<?
 
//Filtra municípios
if ($_REQUEST['filtraMunicipio'] && $_REQUEST['estuf']) {
	//header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipio($_REQUEST['estuf']);
	exit;
}

//pega perfil do usuario
$pflcod = pegaPerfil($_SESSION['usucpf']);


include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo = "PSE - Programa Saúde na Escola";
monta_titulo( $titulo, '' );

if($pflcod == SECRETARIA_ESTADUAL || $pflcod == SECRETARIA_MUNICIPAL || 
	$pflcod == ESCOLA_MUNICIPAL || $pflcod == ESCOLA_ESTADUAL || $pflcod == DIRETOR_ESCOLA || 
	$pflcod == EDUCADOR_ESCOLA || $pflcod == MEC || $pflcod == SUPER_USUARIO || $pflcod == CONSULTA){
?>
<table width="95%" align="center" cellspacing="1" cellpadding="3">
	<tr>
		<td align="right">
			<div style="float:center;position:relative;width:300px;">
					<fieldset style="background-color:#ffffff;">
					<legend>Manual de Instruções</legend>
					<table width="100%" >
						<tr>
							<td valign="bottom" height="50%" style="padding-right:15px;">
								<?
								if($pflcod == SECRETARIA_ESTADUAL || $pflcod == SECRETARIA_MUNICIPAL){
									echo '<a href="/pse/arquivos/instrutivo_liberar_senha.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Liberar Senha</a>';
									echo '<br><a href="/pse/arquivos/instrutivo_saude.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Monitoramento de Ações</a>';
								}
								elseif($pflcod == ESCOLA_MUNICIPAL || $pflcod == ESCOLA_ESTADUAL){
									echo '<a href="/pse/arquivos/instrutivo_saude.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Monitoramento de Ações</a>';
								}
								elseif($pflcod == DIRETOR_ESCOLA){
									echo '<a href="/pse/arquivos/instrutivo_diretor.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Monitoramento de Ações</a>';
								}
								elseif($pflcod == EDUCADOR_ESCOLA){
									echo '<a href="/pse/arquivos/instrutivo_educador.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Monitoramento de Ações</a>';
								}
								elseif($pflcod == MEC || $pflcod == SUPER_USUARIO || $pflcod == CONSULTA){
									echo '<a href="/pse/arquivos/instrutivo_liberar_senha.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Liberar Senha</a>';
									echo '<br><a href="/pse/arquivos/instrutivo_saude.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Perfil Saúde</a>';
									echo '<br><a href="/pse/arquivos/instrutivo_diretor.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Perfil Diretor</a>';
									echo '<br><a href="/pse/arquivos/instrutivo_educador.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Perfil Educador</a>';
								}
								echo '<br><a href="/pse/arquivos/instrutivo_financeiro.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Incentivo Financeiro (Anexo I)</a>';
								echo '<br><a href="/pse/arquivos/instrutivo_financeiro2.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Incentivo Financeiro (Anexo II)</a>';
								echo '<br><a href="/pse/arquivos/instrutivo_financeiro3.pdf" target="_blank" title="Clique aqui para baixar o arquivo">- Instrutivo Incentivo Financeiro (Anexo III)</a>';
								?>
							</td>
						</tr>
					</table>
					</fieldset>
			</div>
		</td>
	</tr>
</table>
<?
}

//$db->cria_aba( $abacod_tela, $url, '' );

$menu = carregarMenuAbasPse();
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);


monta_titulo('Lista de Escolas', 'Filtro de Pesquisa');

$where = "";
if ($_POST){
	if(!empty($_REQUEST['entid'])){
		
		$_SESSION['pse']['entid'] = $_REQUEST['entid'];
		$_SESSION['pse']['sse'] = "N";
		
		$sql = "SELECT muncod FROM entidade.endereco WHERE entid = '{$_SESSION['pse']['entid']}'";
		$_SESSION['pse']['muncod'] = $db->pegaUm($sql);
		 
		echo "<script>window.location.href='pse.php?modulo=principal/cadastroMonitoramento&acao=A';</script>";
		exit;
	}
	$where = ($_POST["entnome"]) ? " AND UPPER(ent.entnome) like UPPER('%".$_POST["entnome"]."%')" : "";
	$where.=($_POST["estuf"]) ? " AND ende.estuf = '".$_POST["estuf"]."'" : "";
	$where.=($_POST["muncod"]) ? " AND ende.muncod = '".$_POST["muncod"]."'" : "";
	$where.=($_POST["entcodent"]) ? " AND ent.entcodent = '".$_POST["entcodent"]."'" : "";
}


//cria sessoes para o pse (nao retirar a linha abaixo)
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';

?>
<form id="formPesquisaEscola" name="formPesquisaEscola" method="post" action="">
	<input type="hidden" name="entid" id="entid">

<?
if ($pflcod != ESCOLA_MUNICIPAL && $pflcod != ESCOLA_ESTADUAL && $pflcod != DIRETOR_ESCOLA && $pflcod != EDUCADOR_ESCOLA){
	?>
	
	<table id="pesquisa" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
		<tr>
			<td width="35%" class="SubTituloDireita" valign="top"><b>INEP:</b></td>
			<td>
			<? 
				$entcodent = $_REQUEST["entcodent"];
				echo campo_texto('entcodent', 'N', 'S', 'Código INEP', 50, 500, '', '');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top"><b>Nome da Escola:</b></td>
			<td>
			<? 
				$entnome = $_REQUEST["entnome"];
				echo campo_texto('entnome', 'N', 'S', 'Nome da Escola', 50, 500, '', '');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top"><b>UF:</b></td>
			<td>
				<?
					$estuf = ($uf==''?$_REQUEST['estuf']:$uf); 
					$sql = "SELECT
								estuf AS codigo,
								estdescricao AS descricao
							FROM
								territorios.estado
							ORDER BY
								estdescricao";
					$db->monta_combo( "estuf", $sql, $comboUFhab, 'Selecione...', 'filtraMunicipio', '', '', '215','','','',$estuf );
	
	 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top"><b>Município:</b></td>
			<td id="td_municipio">
			<? 
				$muncod = ($muncod==''?$_REQUEST['muncod']:$muncod); 
				$sql = "SELECT
							ter.muncod AS codigo,
							ter.mundescricao AS descricao
						FROM
							territorios.municipio ter
						WHERE
							ter.estuf = '$estuf' 
						ORDER BY ter.estuf, ter.mundescricao"; 
				$db->monta_combo( "muncod", $sql, $comboMUNhab, 'Selecione...', '', '', '', '215', 'N','','','',$muncod);
			?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#c0c0c0"></td>
			<td align="left" bgcolor="#c0c0c0">
				<input type="button" id="bt_pesquisar" <?=$btHab;?> value="Pesquisar" onclick="pesquisar()" />
			</td>
		</tr>
	</table>
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td align="center" colspan="2"><b>Lista de Escolas</b></td>
		</tr>
	</table>
	<?
}

?>
</form>
<?

/*
$sql = "SELECT DISTINCT
			'<a href=\"#\" onclick=\"EditarCadastro(\'' || ent.entid || '\');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
			' as acao,
			 ent.entnome, m.estuf, m.mundescricao  
		FROM entidade.entidade ent				
		INNER JOIN
			entidade.endereco ende ON ende.entid = ent.entid
		INNER JOIN
			entidade.funcaoentidade fe ON fe.entid = ent.entid
		INNER JOIN
			territorios.municipio m ON m.muncod = ende.muncod";
*/
$sql = "SELECT DISTINCT
			'<a href=\"#\" onclick=\"EditarCadastro(\'' || ent.entid || '\');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
			' as acao,
			 ent.entnome, m.estuf, m.mundescricao
		FROM entidade.entidade ent				
		INNER JOIN
			entidade.endereco ende ON ende.entid = ent.entid
		INNER JOIN
			entidade.funcaoentidade fe ON fe.entid = ent.entid
		INNER JOIN
			territorios.municipio m ON m.muncod = ende.muncod
		WHERE	ent.entstatus='A' AND 
				fe.funid = 3 AND
				ent.tpcid in (1, 2, 3)
				$where
		ORDER BY m.estuf,m.mundescricao,ent.entnome";

$cabecalho 		= array( "Ação", "Escola", "UF", "Município");
$tamanho		= array( '10%', '50%', '10%', '30%');
$alinhamento	= array( 'center', 'left', 'center', 'center');

if($pflcod == MEC || $pflcod == SUPER_USUARIO){
	if($_POST){
		$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinhamento);
	}
}
else{
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinhamento);
}

//menssagem de aviso	
if(!$_POST){
	$texto = "<div style=\"font-size:12px\" >
					
					<center><b><font color=red><p>SEMANA SAÚDE NA ESCOLA</p></font></b></center>
					
					<div style=\"margin: 0 auto; padding: 0; height: 260px; width: 100%; border: none;\" class=\"div_rolagem\">
						<p>					
							<b>Em 2013, todos os municípios brasileiros poderão aderir ao Programa Saúde na Escola - PSE e à Semana Saúde na Escola, mesmo aqueles que não alcançaram as metas pactuadas em 2012.</b>
						</p>
						<p> 
							O momento é de adesão à Semana Saúde na Escola.<font color=red> A adesão poderá ser feita pelo Secretário de Saúde do Município, no período de 20 de fevereiro a 15 de março de 2013</font>, no seguinte endereço eletrônico: 
							<a href=\"http://dab.saude.gov.br/sistemas/sgdab\" target=\"_blank\">dab.saude.gov.br/sistemas/sgdab</a>.
						</p>
						<p>
							O objetivo principal da Semana Saúde na Escola é dar início a uma mobilização temática prioritária de saúde, que deverá ser trabalhada ao longo do ano letivo nas escolas. Seus objetivos específicos visam:
						</p>
						 
						<p>
							<b>&nbsp; &nbsp; a.</b> Fortalecer ações prioritárias de política de governo, no âmbito da saúde e da educação;
							<br>
							<b>&nbsp; &nbsp; b.</b> Socializar as ações e compromissos do PSE nos territórios;
							<br>
							<b>&nbsp; &nbsp; c.</b> Fortalecer o Sistema de Monitoramento e Avaliação do PSE (E-SUS/SIMEC) como sistema de informação, gestão, monitoramento e avaliação do PSE e da saúde dos educandos;
							<br>
							<b>&nbsp; &nbsp; d.</b> Incentivar a integração e a articulação das redes de educação e atenção básica;
							<br>
							<b>&nbsp; &nbsp; e.</b> Fortalecer a comunicação entre escolas, equipes de Saúde da Família e unidades de saúde;
							<br>
							<b>&nbsp; &nbsp; f.</b> Socializar as ações desenvolvidas pelas escolas;
							<br>
							<b>&nbsp; &nbsp; g.</b> Fomentar o envolvimento da comunidade escolar e de parcerias locais;
							<br>
							<b>&nbsp; &nbsp; h.</b> Mobilizar as redes de atenção à saúde para as ações do PSE.
						</p>
						
						<p>
							A Semana Saúde na Escola compreenderá ações de atenção à saúde dos escolares e de promoção da saúde, cuja mobilização acontecerá no período de 11 a 15 de março de 2013, envolvendo intersetorialmente o planejamento das redes de educação básica e atenção básica em saúde. 
						</p>
						
						<p>
							A Semana inaugura a execução das metas pactuadas no PSE, pois as ações serão contabilizadas para o alcance das metas acordadas pelos municípios e o Distrito Federal no Termo de Compromisso, possibilitando maior visibilidade e o reconhecimento das ações planejadas e executadas no âmbito do Programa, além do fortalecimento da integração e articulação entre os setores da saúde e da educação em nível local.
						</p>
						
						<p>
							Quaisquer informações adicionais podem ser obtidas nos telefones (61) 33159091/9068/9057 (PSE/MS) ou  2022-9298/9209/9216/8328/7939 (PSE/MEC) ou por e-mail: e-mail: <a href=\"mailto:pse@saude.gov.br\">pse@saude.gov.br</a> e <a href=\"mailto:pse@mec.gov.br\">pse@mec.gov.br</a>
						</p>
						
						<b>
						<br> Atenciosamente,
						<br> Equipe Saúde na Escola.
						</b>
						
					</div>
					
		 		  </div>";
	//popupAlertaGeral($texto,'640px',"330px");
}
?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

function filtraMunicipio(estuf) {
	if(estuf!=''){
		var destino = document.getElementById("td_municipio");
		var myAjax = new Ajax.Request(
			window.location.href,
			{
				method: 'post',
				parameters: "filtraMunicipio=true&" + "estuf=" + estuf,
				asynchronous: false,
				onComplete: function(resp) {
					if(destino) {
						destino.innerHTML = resp.responseText;
					} 
				},
				onLoading: function(){
					destino.innerHTML = 'Carregando...';
				}
			});
	}
}


var btPesquisa	= document.getElementById("bt_pesquisar");

function pesquisar() {
	<?if($pflcod == MEC || $pflcod == SUPER_USUARIO){?>
		if( document.formPesquisaEscola.entcodent.value == '' && 
			document.formPesquisaEscola.entnome.value == '' &&
			document.formPesquisaEscola.estuf.value == '' &&
			document.formPesquisaEscola.muncod.value == ''){
			alert("Preencha pelo menos um item para pesquisar.");
			return false;
		}
	<?}?>
	
	btPesquisa.disabled = true;
	document.formPesquisaEscola.submit();
}

function EditarCadastro(entid) {
	document.getElementById("entid").value=entid;
	document.formPesquisaEscola.submit();
}

</script>