<?php


if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br/>";

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function setarInuId(estuf) {
	window.location='snf.php?modulo=principal/diagnosticoPar&acao=A&requisicao=setarInuIdPorEstuf&estuf='+estuf+'&goto=ide';
}
</script>
<?

$perfis = pegaPerfilGeral();

if((in_array(PERFIL_FORUM_APROVACAO,$perfis) || in_array(PERFIL_FORUM_DISTRIBUICAO,$perfis))) {
	
	$sql = "SELECT DISTINCT estuf FROM snf.usuarioresponsabilidade WHERE pflcod IN('".PERFIL_FORUM_APROVACAO."','".PERFIL_FORUM_DISTRIBUICAO."') AND usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'";
	$estuf = $db->pegaUm($sql);
	
	if($estuf) setarInuIdPorEstuf(array("estuf" => $estuf));
	
}

/* montando o menu */
echo montarAbasArray(montaMenuAtendimentoForum(), $_SERVER['REQUEST_URI']);
/* fim montando o menu */

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<thead>
		<th class="tdDegradde01" colspan="2" align="left">
			<h1 class="notprint">
				PARFOR
			</h1>
		</th>
	</thead>

	<tr>
		<td>

<p>O Plano Nacional de Formação de Professores da Educação Básica – PARFOR visa induzir e fomentar a oferta emergencial de vagas em cursos de educação superior, gratuitos e de qualidade, nas modalidades presencial e a distância, para professores em exercício na rede pública de educação básica, a fim de que estes profissionais possam ter acesso à formação exigida pela Lei de Diretrizes e Bases da Educação Nacional – LDBEN, contribuam para a melhoria da qualidade da educação básica.</p>  
<p>O acesso dos docentes aos cursos do Parfor é realizado por intermédio da oferta de turmas em cursos de licenciatura, programas de segunda licenciatura – ofertado somente na modalidade a presencial - e formação pedagógica das Instituições de Educação Superior – IES.</p>
<p>Podem se pré-inscrever nos cursos de licenciatura, docentes em exercício na rede pública da educação básica que não tenham formação superior, ou que, mesmo tendo essa formação, se disponham a realizar o curso de licenciatura na área em que atua em sala de aula.</p>
<p>Nos Programas de Segunda Licenciatura, podem se pré-inscrever docentes que já possuam formação em licenciatura, mas que atuem em área distinta dessa formação. Nesse caso os docentes deverão comprovar ter pelos menos três anos de exercício no magistério na educação básica e realizar sua pré-inscrição no curso correspondente à disciplina que ministra em sala de aula.</p> 
<p>Nos Programas de Formação pedagógica, podem se pré-inscrever docentes graduados não licenciados que se encontram no exercício da docência na rede pública da educação básica.</p>
<p>Para concorrer à vaga nos cursos ofertados, os docentes devem: a) realizar seu cadastro e pré- inscrição na Plataforma Freire; b) estar cadastrado no Educacenso na função docente da rede pública de educação básica; e ter sua pré-inscrição validada pela Secretaria de educação ou órgão equivalente a  que estiver vinculado.</p>
<p>A pré-inscrição não garante a matrícula. A efetivação da matrícula está condicionada à comprovação dos requisitos para a participação e às normas e procedimentos acadêmicas da Instituição de Educação Superior - IES ofertante do curso.</p>
<p>Para informações sobre calendário clique no link <a href="http://freire.mec.gov.br/index/calendario" target="_blank">http://freire.mec.gov.br/index/calendario</a>.</p>

		</td>
	</tr>
</table>

