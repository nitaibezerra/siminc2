<?php
require_once APPRAIZ . "includes/classes/entidades.class.inc";

$_SESSION['academico']['entid'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entid'];
$entid = $_SESSION['academico']['entid'] ? $_SESSION['academico']['entid'] : $_REQUEST['entid'];
$orgid = $_SESSION['academico']['orgid'];

// instancia o objeto do módulo 
$academico = new academico();

$perfilNotBloq = array(//PERFIL_IFESCADBOLSAS, 
					   PERFIL_CADASTROGERAL, 
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR,
					   PERFIL_REITOR,
					   PERFIL_ALTA_GESTAO,
					   PERFIL_ASSESSORIA_ALTA_GESTAO);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);

$bloqueado = $permissoes['gravar'] ? false :  true;
$bloq = '';
$hide = '';
if($bloqueado) {
	$bloq = 'disabled="disabled"';
	$hide = 'hide';
}
// Busca a unidade pai do Campus
//$autoriazacaoconcursos = new autoriazacaoconcursos();
//$unidade = ($_REQUEST['entidunidade'] || $_SESSION['academico']['entid']) ? $entid : $autoriazacaoconcursos->buscaentidade($entid);

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$unidade = $entid;
if($unidade) {
	$_SESSION['academico']['entid'] 		= $entid;
	$_SESSION['academico']['entidadenivel'] = 'unidade';
	$_SESSION['sig_var']['entid'] 			= $_SESSION['academico']['entid'];
	$_SESSION['sig_var']['iscampus'] 		= 'nao';
	
	$edtdsc = $db->pegaUm("SELECT entd.edtdsc FROM academico.entidadedetalhe entd WHERE entd.entid='".$_SESSION['sig_var']['entid']."'");
	$edtdsc = simec_htmlspecialchars(str_replace(array("\r\n","\n\r","\r","\n"), '\n', $edtdsc), ENT_QUOTES);
}

// Testando se a varival $unidade esta correta (feito pelo Alexandre para mapear erros ocorridos)
if(!$unidade) {
	echo "<script>alert('Unidade não selecionada.');window.location='/academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_profissional&acao=A';</script>";
	exit;
}

$url = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A';
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Consolidação e Expansão IFES - Campus/Reitoria", "");

$orgid = $_SESSION['academico']['orgid'];
$orgao = "Educação Profissional";

$sql = "SELECT 
			ent.entid as unidadeorcid, 
			ent.entnome as unidadeorc, 
			ende.estuf, 
			mundescricao,
			entunicod 
		FROM 
			entidade.entidade ent
		LEFT JOIN entidade.endereco 	ende ON ende.entid = ent.entid 
		LEFT JOIN territorios.municipio mun  ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
		WHERE 
			ent.entid = '". $entid ."' 
		ORDER BY 
			ent.entnome";
	
$dados = $db->pegaLinha( $sql );

$cabecalho = '';
if($dados) {
	$cabecalho = "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>"
	. "<tr>"
	. "<td class='SubTituloDireita' width='250px;'>Tipo Ensino:</td><td>".$orgao."</td>"
	. "</tr>"
	. "<tr>"
	. "<td class='SubTituloDireita'>Instituição:</td><td>".$dados['unidadeorc']." - ".$dados['entunicod']."</td>"
	. "</tr>"
	. "<tr>"
	. "<td class='SubTituloDireita'>UF / Munícipio:</td><td>".$dados['estuf']." / ".$dados['mundescricao']."</td>"
	. "</tr>"
	. "</table>";
} else {
	$cabecalho .=("<script>
			alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
			window.location='?modulo=inicio&acao=C';
		 </script>");
}
		   
echo $cabecalho;	
?>
<form name="formulario" method="POST" action="">
	<center>
		<fieldset style="background: #fff; width: 93%;">
			<legend align="center">Lista de Campus</legend>
	<?	
		$funid = ACA_ID_CAMPUS; 
			
		$sql = "SELECT 
					'<a style=\"cursor:pointer;\" onclick=\"abreQuestionarioCampus(' || e.entid || ');\">' || upper(e.entnome) || '</a>' as nome,
					upper(mun.mundescricao) as municipio, 
					upper(mun.estuf) as uf
				FROM
					entidade.entidade e2
				INNER JOIN entidade.entidade 		e 	ON e2.entid = e.entid
				INNER JOIN entidade.funcaoentidade 	ef  ON ef.entid = e.entid
				INNER JOIN entidade.funentassoc 	ea  ON ea.fueid = ef.fueid 
				LEFT  JOIN entidade.endereco 		ed  ON ed.entid = e.entid
				LEFT  JOIN territorios.municipio 	mun ON mun.muncod = ed.muncod
				WHERE
					ea.entid = {$unidade} AND e.entstatus = 'A' AND ef.funid = {$funid}
				ORDER BY
					e.entnome";
		
		$cabecalho = array('Campus', 'Município', 'UF');
		$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '90%');
	?>
		</fieldset>
	</center>
</form>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
function abreQuestionarioCampus( entid ){
	return window.location = '?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&entid=<?=$unidade ?>&entidcampus=' + entid;
}

$(document).ready(function(){
	$('#aguarde').hide();
	$('#btnGravar').click(function(){
		var vazio = true;
		$('[name="resdescricao"]').each(function(){
			if( $(this).attr('checked') ){
				vazio = false;
			}else{
				$(this).focus();
			}
		});
		$('.obrigatorio').each(function(){
			if( $(this).val() == '' ){
				$(this).focus();
				vazio = true;
				return false;
			}
		});
		if( vazio ){
			alert('Pergunta não respondida.');
			return false;
		}
		$('[name="req"]').val('gravar1');
		$('[name="formulario"]').submit();
	});
	$('#btnProx').click(function(){
		$('[name="req"]').val('proximo1');
		$('[name="formulario"]').submit();
	});
});

</script>