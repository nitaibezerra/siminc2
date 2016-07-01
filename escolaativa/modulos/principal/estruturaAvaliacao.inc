<?php
/**
 * Sistema Simec
 * Setor responsável: MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Luiz Fernando Mendonça
 * Programador: Gustavo Fernandes da Guarda
 */ 
include_once APPRAIZ . "includes/workflow.php";
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$boEstrutura = true;

echo montarAbasArray( criaAbaEscolaAtiva($boEstrutura), "escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" );
monta_titulo( $titulo_modulo, '' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />
<link rel="stylesheet" href="../includes/jquery-treeview/red-treeview.css" />
<link rel="stylesheet" href="screen.css" />
<link rel="stylesheet" type="text/css" href="../includes/jquery-modal/jqModal.css" rel="stylesheet" />
<style type="text/css">

div.jqmWindow {
	background: #fff;
}


div.fechar .jqmClose {
  width:20px;
  height:20px;
  display:block;
  float:right;
  clear:right;
  background:transparent url(../includes/jquery-modal/close_icon_double.png) 0 0 no-repeat;
  
}

div.fechar a.jqmClose:hover{ background-position: 0 -20px; }

</style>
<script type="text/javascript" src="../includes/jquery-treeview/lib/jquery.cookie.js"></script>
<!--<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview.js"></script>-->
<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview-simec.js"></script>
<script type="text/javascript" src="../includes/jquery-modal/jqModal.js"></script>
<script type="text/javascript">
<!--
$(document).ready(function(){
	$("#_arvore").treeview({
		collapsed: true,
		animated: "medium",
		control:"#sidetreecontrol",
		persist: "cookie"
		
	});

	$('#abrirTodos').click();

	$('#popupMensalBimestral').jqm({ajax: '@data-ajax-url' , trigger: 'img.abreMensalBimestral', modal:true , overlay: 80});

});

function questionario(monid,monperiodicidade, tecid, tipo){
	if(tipo == 'municipio'){
		var muncod = '<?php echo $_GET['muncod']; ?>';
		window.open('escolaativa.php?modulo=principal/questionario&acao=A&monid='+monid+'&monperiodicidade='+monperiodicidade+'&tecid='+tecid+'&muncod='+muncod,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=950,height=700').focus();
	} else if(tipo == 'estado'){
		var estuf = '<?php echo $_GET['estuf']; ?>';
		window.open('escolaativa.php?modulo=principal/questionario&acao=A&monid='+monid+'&monperiodicidade='+monperiodicidade+'&tecid='+tecid+'&estuf='+estuf,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=950,height=700').focus();
	}
	return false;
}
//-->
</script>
<?php 

$texto = "<div style=\"font-size:12px\" >Senhores (as) Usuários (as),<br /><br />
Informamos que os Cadernos de Ensino e Aprendizagem, com adequações, do Programa Escola Ativa, constante deste módulo, encontra-se disponível para download.<br/>
Cadernos de Ensino e Aprendizagem, com adequações, do Programa Escola Ativa: <a target=\"_blank\" href=\"escolaativa.php?modulo=sistema/geral/downloads_usu&acao=A\" >clique aqui</a>.</div>";
popupAlertaGeral($texto,'450px',"250px");


	$aMeses = array(1=>'Janeiro',
					2=>'Fevereiro',
					3=>'Março',
					4=>'Abril',
					5=>'Maio',
					6=>'Junho',
					7=>'Julho',
					8=>'Agosto',
					9=>'Setembro',
					10=>'Outubro',
					11=>'Novembro',
					12=>'Dezembro');
					
    extract($_GET);
    
    $where = "";
    # Verifica Perfils
    if($_SESSION['escolaativa']['montipo'] == 'M'){
	    if($_SESSION['escolaativa']['muncod']){
	    	$muncod = $_SESSION['escolaativa']['muncod'];
	    	$where = " and e.entid = {$_SESSION['escolaativa']['entid']} ";
	    }	
    } elseif($_SESSION['escolaativa']['montipo'] == 'E'){
     	if($_SESSION['escolaativa']['estuf']){
     		if($_SESSION['escolaativa']['tecid'] && is_numeric($_SESSION['escolaativa']['estuf'])){
    			$where = " and t.tecid = {$_SESSION['escolaativa']['tecid']} ";
     		}
    		$estuf = $_SESSION['escolaativa']['estuf'];
    	}
    }
    
    if($_SESSION['escolaativa']['boPerfilMultiplicador']){
   		$tituloDiagnostico = "Diagnóstico Municípal";
    } else {
	    if($_SESSION['escolaativa']['montipo'] == 'M'){
	    	$tituloDiagnostico = "Diagnóstico Municípal";

	    	$sql = "select mundescricao, e.estdescricao from territorios.municipio m
						inner join territorios.estado e on m.estuf = e.estuf
					where muncod = '$muncod'";
		    $aMunicipio = $db->pegaLinha($sql);
		    
		    $munestdescricao = $aMunicipio['mundescricao'];
		    $estdescricao    = $aMunicipio['estdescricao'];
	    } else {
	    	$tituloDiagnostico = "Diagnóstico Estadual";
	    	$sql = "select estdescricao from territorios.estado where estuf = '$estuf'";
		    $munestdescricao = $db->pegaUm($sql);
	    }    	
    }
    
    /**
     * Bloco para verificar o perfil e pega os Porofessores Multiplicadores conforme o perfil
     */    
    /*if($_SESSION['escolaativa']['boPerfilPesquisador']){ # Se Perfil pesquisador
    	$sql = "select e.entid, e.entnome, t.tecid from escolaativa.tecnico t
				inner join entidade.entidade e on t.entid = e.entid";
    	$aProfessorMultiplicador = $db->carregar($sql);
    } else*/
    if($_SESSION['escolaativa']['boPerfilMultiplicador']){ # Se Perfil multiplicador
    	$sql = "select 
	    				t.tecid,
		    			e.entid,
		    			e.entnome
					from escolaativa.tecnico t
						inner join entidade.entidade e on t.entid = e.entid
			    	where e.entnumcpfcnpj = '{$_SESSION['usucpf']}'";
    		$aProfessorMultiplicador = $db->carregar($sql);
    } else { # Senão verifica se o montipo é municipal ou estadual 
	    if($_SESSION['escolaativa']['montipo'] == 'M'){
	    	$sql = "select 
	    				distinct
	    				t.tecid,
		    			e.entid,
		    			e.entnome
					from escolaativa.tecnico t
						inner join entidade.entidade e on t.entid = e.entid
						inner join escolaativa.tecnicoresponsabilidade tr on t.tecid = tr.tecid
						inner join escolaativa.usuarioresponsabilidade ur on e.entnumcpfcnpj = ur.usucpf and ur.rpustatus = 'A'
			    	where tr.muncod = '$muncod' and ur.pflcod = ".ESCOLAATIVA_PERFIL_MULTIPLICADOR." $where ";
	    	$aProfessorMultiplicador = $db->carregar($sql);
	    } else {
	    	$sql = "select 
					    distinct
					    t.tecid,
					    e.entid,
					    e.entnome
					from escolaativa.tecnico t
					    inner join entidade.entidade e on t.entid = e.entid
					    --inner join escolaativa.tecnicoresponsabilidade tr on t.tecid = tr.tecid
					    inner join escolaativa.usuarioresponsabilidade ur on e.entnumcpfcnpj = ur.usucpf and ur.rpustatus = 'A'
					where ur.pflcod = ".ESCOLAATIVA_PERFIL_SUPERVISOR."
					and ur.estuf = '$estuf' $where ";
		    $aSupervisor = $db->carregar($sql);
		    $aSupervisor = ($aSupervisor) ? $aSupervisor : array();
	    }
    }
    
    $aProfessorMultiplicador = ($aProfessorMultiplicador) ? $aProfessorMultiplicador : array();
    
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td width="100%" valign="top" style="background: none repeat scroll 0% 0%;" id="_arvore">
			<div id="main" style="background: #f5f5f5">
				<div id="sidetree">
					<div class="treeheader">&nbsp;</div>
					<div id="sidetreecontrol"><span><a href="?#">Fechar Todos</a> | <a href="?#" id="abrirTodos">Abrir Todos</a> <br /><br /> <img src="../includes/jquery-treeview/images/base.gif" align="top" /><?php echo $tituloDiagnostico; ?></span></div>
					<!-- VERIFICA O PERFIL DE MULTIPLICADOR -->
					<?php if($_SESSION['escolaativa']['boPerfilMultiplicador']): ?>
						<?php arvoreProfessorMultiplicador($aProfessorMultiplicador, $muncod, $aMeses); ?>
					<?php else: ?> <!-- FIM DO PERFIL MULTIPLICADOR -->
						<?php if($_SESSION['escolaativa']['montipo'] == 'M'): ?>
							<?php arvoreMunicipio($aMunicipio, $aProfessorMultiplicador, $muncod, $aMeses );?>
						<?php elseif($_SESSION['escolaativa']['montipo'] == 'E'): ?>
							<?php arvoreEstado($munestdescricao, $aSupervisor, $estuf, $aMeses );?>
						<?php endif;?>
					<?php endif;?>
				</div><!-- div #sidetree -->
			</div><!-- div #main -->
		</td>
	</tr>
</table>
<br />
<div class="jqmWindow" id="popupMensalBimestral"></div>