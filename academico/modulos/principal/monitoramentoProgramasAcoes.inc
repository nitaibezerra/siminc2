<? 
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
if( $_REQUEST['popup'] ){
	$_REQUEST['popup']();
	die();
}

$perfilNotBloq = Array();

// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança


if($_REQUEST['requisicao']) {
	if($_REQUEST['requisicao'] != "cadastrarAutorizacao"){
		$arrDados = $_REQUEST['requisicao']($_REQUEST);
		if(is_array($arrDados)){
			extract($arrDados);
		}
	}
}

if(!$_SESSION['academico']['entid']) {
	
	die("<script>
			alert('Entidade não encontrada. Navegue novamente.');
			window.location='academico.php?modulo=inicio&acao=C';
		 </script>");
	
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

unset($_SESSION['arrayObras']);

#$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo('Monitoramento de Programas e Ações', $linha2);
#academico_monta_cabecalho_sig($_SESSION['academico']['entid'], 'Monitoramento de Programas e Ações');
$autoriazacaoconcursos = new autoriazacaoconcursos();

?>
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
<?php
    echo $autoriazacaoconcursos->cabecalho_entidade($_SESSION['academico']['entid']);
$aba = !$_GET['aba'] ? "Infraestrutura" : $_GET['aba'];

$abaAtiva = "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=$aba";

/*** Array com os itens da aba de identificação ***/
$menu = array( 0 => array("id" => 1, "descricao" => "Demanda/Infraestrutura", "link" => "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=Infraestrutura")
			  ,1 => array("id" => 2, "descricao" => "Expansão Ensino Médico", "link" => "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=ExpansaoEM")
			  ,2 => array("id" => 3, "descricao" => "Procampo", "link" => "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=PROCAMPO")
			  ,3 => array("id" => 4, "descricao" => "Educação Bilíngue", "link" => "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=EduBilingue")
			  ,4 => array("id" => 5, "descricao" => "Bilingue-INES", "link" => "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=BilingueInes")
			  ,5 => array("id" => 6, "descricao" => "Câmpus com Menos de 5 Cursos", "link" => "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=CampusCursos")
			  ,6 => array("id" => 7, "descricao" => "IndicadoresAcademicos", "link" => "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=IndicadoresAcademicos")
			  ,7 => array("id" => 8, "descricao" => "Adesão (SISU-EBSERH)", "link" => "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=Adesao")
			  ,8 => array("id" => 9, "descricao" => "Novos Campus", "link" => "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=NovosCampus")
			  ,9 => array("id" => 10, "descricao" => "Nova Universidade", "link" => "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=NovaUniversidade")
			  ,10 => array("id" => 11, "descricao" => "Programa Incluir", "link" => "academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=ProgramaIncluir")
);
echo "<br/>"; 
echo montarAbasArray($menu, $abaAtiva);			  

include_once "mpa/mpa".$aba.".inc";
?>
</div>