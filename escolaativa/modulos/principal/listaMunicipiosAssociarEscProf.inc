<?php
/**
 * Sistema Simec
 * Setor responsável: MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Luiz Fernando Mendonça
 * Programador: Gustavo Fernandes da Guarda
 */
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

if(!count($_SESSION['escolaativa']['arMuncod']) && $_SESSION['escolaativa']['muncod']){
	header( "Location: escolaativa.php?modulo=principal/listaMunicipioProfessor&acao=A&muncod=".$_SESSION['escolaativa']['muncod'] );
	exit;
}
	

//$_SESSION['escolaativa']['montipo'] = "M";

//echo montarAbasArray( criaAbaEscolaAtiva(), "escolaativa.php?modulo=principal/listaMunicipios&acao=A" );

$titulo_modulo = 'Associar Escola Professor';
monta_titulo( $titulo_modulo, '' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
function abreArvore(muncod){
	if(muncod){
		//window.open('escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A&muncod='+muncod,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=700,height=815');
		window.location.href = 'escolaativa.php?modulo=principal/listaMunicipioProfessor&acao=A&muncod='+muncod;
	}
}
</script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form method="post" name="formulario" id="formulario">
					<input type="hidden" name="pesquisa" value="1>"/>
					<div style="float: left;">
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									Município
									<br/>
									<?php 
										//$filtro = simec_htmlentities( $_POST['municipio'] ); 
										$municipio = $_POST['municipio'];
										echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', '' ); 
									?>
								</td>
								<td valign="bottom">
									Estado
									<br/>
									<?php
									$estuf = $_POST['estuf'];
									$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
									$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
									?>
								</td>
							</tr>
							<tr>
								<td colspan="2">
									<br />
									<label><input type="checkbox" name="capitais" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['capitais'] == 1 ? 'checked="checked"' : '' ?>/> Capitais</label>
									<label><input type="checkbox" name="grandescidades" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['grandescidades'] == 1 ? 'checked="checked"' : '' ?>/> Grandes Cidades</label>
									<label><input type="checkbox" name="indigena" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['indigena'] == 1 ? 'checked="checked"' : '' ?>/> Comunidade Indígena</label>
									<label><input type="checkbox" name="quilombola" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['quilombola'] == 1 ? 'checked="checked"' : '' ?>/> Comunidade Quilombola</label>
									<label><input type="checkbox" name="cidadania" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['cidadania'] == 1 ? 'checked="checked"' : '' ?>/> Territórios da Cidadania</label>
									<?php
									//$analise = $_POST["analise"];
									?>
									<!--<label style="font-weight: bold;" for="analise_todos">
										<input
											type="radio"
											name="analise"
											value=""
											id="analise_todos"
											<?= $analise == "" ? 'checked="checked"' : '' ?>
											onchange="alterarFiltroAnalise();"
											onclick="alterarFiltroAnalise();"
										/>Todas
									</label>
									<label for="analise_naoanal"><input id="naoanalisado" type="radio" name="analise" value="naoanalisado" <?= $analise == "naoanalisado" ? 'checked="checked"' : '' ?> onchange="alterarFiltroAnalise();" onclick="alterarFiltroAnalise();"/>Não Analisado</label>
									<label for="analise_emanal"><input id="emanalise" type="radio" name="analise" value="emanalise" <?= $analise == "emanalise" ? 'checked="checked"' : '' ?> onchange="alterarFiltroAnalise();" onclick="alterarFiltroAnalise();"/>Em Análise</label>
									<label for="analise_anal"><input  id="analisado" type="radio" name="analise" value="analisado" <?= $analise == "analisado" ? 'checked="checked"' : '' ?> onchange="alterarFiltroAnalise();" onclick="alterarFiltroAnalise();"/>Analisado</label>
								--></td>
								<td>
									<!--<label><input type="checkbox" name="capitais" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['capitais'] == 1 ? 'checked="checked"' : '' ?>/> Capitais</label>
									<label><input type="checkbox" name="grandescidades" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['grandescidades'] == 1 ? 'checked="checked"' : '' ?>/> Grandes Cidades</label>
									<br /><br />
									<label><input type="checkbox" name="indigena" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['indigena'] == 1 ? 'checked="checked"' : '' ?>/> Comunidade Indígena</label>
									<label><input type="checkbox" name="quilombola" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['quilombola'] == 1 ? 'checked="checked"' : '' ?>/> Comunidade Quilombola</label>
									<label><input type="checkbox" name="cidadania" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['cidadania'] == 1 ? 'checked="checked"' : '' ?>/> Territórios da Cidadania</label>
								--></td>
							</tr>
						</table>
					<div style="float: left;">
						<input type="submit" name="pesquisar" id="pesquisar" value="Pesquisar" />
					</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>
<?php 
$where = array();
$join_capitais = $join_grandescidades = $join_indigena = $join_quilombola = $join_cidadania = "";

if($_SESSION['escolaativa']['arMuncod']){
	array_push($where, "  m.muncod in ('". implode("','", $_SESSION['escolaativa']['arMuncod'])."') ");
}

if($_POST['pesquisa']){
	extract($_POST);
	
	// Programa
	if( $municipio ){
		$municipioTemp = removeAcentos($municipio);
		array_push($where, " public.removeacento(m.mundescricao) ilike '%".$municipioTemp."%' ");
	}
	// Estado
	if( $estuf ){
		array_push($where, " m.estuf = '".$estuf."' ");
	}
	// Analise
	//if( $analise ){
		//array_push($where, " dc.muncod in ('" . implode( "','", $muncod ) . "') ");
	//}
	
	if ( $capitais ) {
		$join_capitais = "inner join territorios.estado e on e.estuf = m.estuf and e.muncodcapital = m.muncod";
	}
	
	if ( $grandescidades ) {
		$join_grandescidades = "inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = m.estuf and mtm.tpmid = 1";
	}
	
	if ( $indigena ) {
		$join_indigena = "inner join territorios.muntipomunicipio mti on mti.muncod = m.muncod and mti.estuf = m.estuf and mti.tpmid = 16";
	}
	
	if ( $quilombola ) {
		$join_quilombola = "inner join territorios.muntipomunicipio mtq on mtq.muncod = m.muncod and mtq.estuf = m.estuf and mtq.tpmid = 17";
	}
	
	if ( $cidadania ) {
		$join_cidadania = "inner join territorios.muntipomunicipio mtq2 on mtq2.muncod = m.muncod and mtq2.estuf = m.estuf and mtq2.tpmid = 139";
	}

}
	
$sql = "select '<center>
					<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreArvore('|| m.muncod ||')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
				</center>' as acao, 
				m.muncod, m.mundescricao, m.estuf 
		from territorios.municipio m
		$join_capitais
		$join_grandescidades
		$join_indigena
		$join_quilombola
		$join_cidadania		
		" . ( is_array($where) && count($where) ? ' where ' . implode(' AND ', $where) : '' ) ."
		order by m.mundescricao
		";
		
//ver($sql);
$cabecalho = array("Ação","Código","Município", "UF");
$db->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2);