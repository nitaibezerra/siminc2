<?php

pegaSessoes();


// objeto da classe do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();

//// Pega as unidades permitidas do usuário
//$unidades_permitidas = academico_pegarUnidadesPermitidas();

// Define as variáveis da tela
$orgid	 	 	= $_SESSION["academico"]["orgid"];
$ano			= $_REQUEST['ano'] ?  $_REQUEST['ano'] : '2008';
$orgdesc 		= ($orgid == 1) ? "Educação Superior" : "Educação Profissional";

$prtid			= $_SESSION["academico"]["prtid"];
$entidcampus 	= $_SESSION["academico"]["entidcampus"];

$prgid 			= $_SESSION["academico"]["prgid"];

# Define como padrão o tipo de edital como de Concursos
$_SESSION["academico"]["tpetipo"] = ACA_TPEDITAL_PUBLICACAO;
//
//$_SESSION["academico"]["carga"] =  $_REQUEST["carga"]?  $_REQUEST["carga"] : $_SESSION["academico"]["carga"];
//$carga = $_SESSION["academico"]["carga"];
//
//// Carrega os campus da undiade
//if ( $_REQUEST["carga"] ){	
//	$autoriazacaoconcursos->listacampusedital($_REQUEST["carga"]);
//	die;
//}


$perfilNotBloq = array(PERFIL_IFESCADBOLSAS, 
					   PERFIL_IFESCADCURSOS, 
					   PERFIL_IFESCADASTRO, 
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança

$habilitado = $permissoes['gravar'];

// carrega as portarias de provimentos
if ( $_REQUEST["cargaportarias"] ){
	$autoriazacaoconcursos->listaportariasprovimentos($_REQUEST["cargaportarias"], $entidcampus);
	die;
}

if(isset($_REQUEST[evento]) && ($_REQUEST[evento] != '')){
	
	switch($_REQUEST[evento]) {
		case 'E':	
			$edpid = $_REQUEST[edpid]; 				
			
			$sql = "SELECT							 
							tpeid
						FROM
							academico.editalportaria
						WHERE
							edpid = {$edpid}";
			$tpetipo = $db->pegaUm($sql);

			$sql = "SELECT							 
							tpeid
						FROM
							academico.editalportaria
						WHERE
							edpidhomo = {$edpid}
							AND edpstatus = 'A'
							";
			$tem_homo = $db->pegaUm($sql);			
			
			
			$sql_pub = "SELECT lp.lepid
							FROM academico.lancamentoeditalportaria AS lp
							WHERE 
							lp.edpid = $edpid AND							
							lp.lepano = '".$ano."'
						";
			$tem_pub = $db->pegaLinha($sql_pub);
			
		
			if (!$tem_homo && !$tem_pub){
				$sql_D = "
						UPDATE academico.editalportaria SET edpstatus = 'I' where edpid = ".$edpid."";	
				$db->executar($sql_D);
				$db->commit();
				$prtid			= $_SESSION["academico"]["prtid"];
				$entidcampus 	= $_SESSION["academico"]["entidcampus"];
				$prgid 			= $_SESSION["academico"]["prgid"];
				
				if ($tpetipo == ACA_TPEDITAL_NOMEACAO )
					$carga="&efetivacao=true";
					
				echo "<script>
						alert('Dados excluidos com sucesso.');
						window.location = '?modulo=principal/listareditais&acao=C&evento=A&entidcampus=$entidcampus&prtid=$prtid&prgid=$prgid$carga';
					  </script>";
			}else if($tem_pub){
				echo "<script>
					alert('Este registro não pode ser excluido pois possui lançamentos de concursos vinculados.');
				  	history.go(-1);
					</script>";
			}else if($tem_homo){
				echo "<script>
					alert('Este registro não pode ser excluido pois possui um edital de homologação vinculado.');
				  	history.go(-1);
					</script>";
			}
		exit;
		break;			
	}
}		
require_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';

// Lista os editais
//if ( $_REQUEST["entidcampus"] && $_REQUEST["prtid"] || 1==1  ){
if ($entidcampus && $prtid){
	
	// Monta as abas
	#$db->cria_aba($abacod_tela,$url,$parametros);

	if ($_REQUEST["efetivacao"]=="true")
		monta_titulo( "Lista de Portarias de Efetivação", "");
	else
		monta_titulo( "Lista de Editais de Concurso", "");
	
	// mensagem de bloqueio
	$bloqueado = academico_mensagem_bloqueio( $_SESSION['academico']['orgid']);
	$habil = $bloqueado ? 'N' : $habil;
	$habilitado = $bloqueado ? false : $habilitado;
	$disabled = $bloqueado ? 'disabled=\"disabled\"' : '';
	
	
?>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
    <? echo $autoriazacaoconcursos->cabecalhoedital_ini($prtid, $entidcampus); ?>
	<form method="POST"  name="formulario">
		<input type="hidden" name="evento" id="evento" value="I">
		<input type="hidden" name="edpid" id="edpid" value="<?=$edpid?>">
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="20%" align='right' class="SubTituloDireita">Número :</td>
		    <td><?=campo_texto('edpnumero','N','S','',20,100,'','');?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Número de Controle:</td>
		    <td><?=campo_texto('edpid_busca','N','S','',20,100,'','','','','','onkeypress="return somenteNumeros(event);"');?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Data de Inclusão:</td>
		    <td><?=campo_data('edpdtinclusao','N','S','','','','');?></td>
		</tr>
		<tr bgcolor="#DCDCDC">
		      <td></td>
		      <td colspan="4">
		  	  	<input type="button" class="botao" name="btassociar" value="Limpar" onclick="limpar();">	  	  
			  	<input type="button" class="botao" name="btassociar" value="Pesquisar" onclick="submeter('P', '<?=$prtid?>');">  	  	 
			  	<span style="float:right;">
			  				      <?
				if(academico_possui_perfil(array(PERFIL_ADMINISTRADOR,PERFIL_INTERLOCUTOR_INSTITUTO,PERFIL_REITOR,PERFIL_PROREITOR))){
                    if ($_REQUEST["efetivacao"]=="true") {
                        echo "<input type=\"button\" class=\"botao\" name=\"btassociar\" value=\"Cadastrar Portaria de Efetivação\"  onclick=\"location.href='academico.php?modulo=principal/cadedital&acao=C&evento=N&tpetipo=5'\" $disabled >";
                    } else {
                        echo "<input type=\"button\" class=\"botao\" name=\"btassociar\" value=\"Cadastrar Edital\"  onclick=\"location.href='academico.php?modulo=principal/cadedital&acao=C&evento=N&tpetipo=2'\" $disabled >";
                    }
                }
				?> 
			  	</span>
		    </td>
		 </tr>
        </table>
	</form>
	
<?php
	
	// Verifica se o usuário tem permissão pra ver o campus
	$possui_permisao = academico_verificapermissao( $prtid, $entidcampus );
	
	if( !$possui_permisao ){
		echo "<script>
				alert('Você não possui permissão para visualizar as portarias deste campus!');
				history.back(-1);
			  </script>";
		die;
	}
	
	// Verifica se existe a portaria
	if ( $prtid ){
		$_REQUEST['prtid'] = $prtid;
		include_once APPRAIZ . "www/academico/_permissoes.php";
	
	}
	
	// Verifica se o programa existe
	$existe_programa = academico_verificaprograma( $prgid );
	
	if( !$existe_programa ){
		echo "<script>
				alert('O programa passado não existe!');
				history.back(-1);
			  </script>";
		die;
	}
	
	//Verifica se o campus exite
	$existecampus = academico_existecampus( $entidcampus );
	
	if( !$existecampus ){
		echo "<script>
				alert('O Campus informado não existe!');
				history.back(-1);
			  </script>";
		die;
	}
	
	// Filtros
	if($_REQUEST['edpnumero']) {				
		$filtros[] = "upper(edp.edpnumero) like upper('%".$_REQUEST['edpnumero']."%')";
	}
	if($_REQUEST['edpid_busca']) {
		$filtros[] = "edp.edpid= ".$_REQUEST['edpid_busca']." ";
	}
	if($_REQUEST['edpdtinclusao']) {
		$filtros[] = "to_char(edp.edpdtinclusao, 'DD/MM/YYYY')= '".$_REQUEST['edpdtinclusao']."' ";
	}		
	
	$filtros[] = " edp.prtid = $prtid ";
	
	if (count($filtros) > 0) 
		$filtros=implode(" AND ", $filtros)." AND ";
	
	$tpeid=ACA_TPEDITAL_PUBLICACAO;	
		
	if ( $_REQUEST["efetivacao"] )
		$tpeid=ACA_TPEDITAL_NOMEACAO;	
	
	$autoriazacaoconcursos->listaeditais($entidcampus, $filtros, $prtid, $tpeid);
    ?>
</div>
    <?php
// Lista as portarias
}

?>
<!-- biblioteca javascript local -->
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

	function apagar_edital(edpid){
		if ( confirm( "Deseja apagar o Edital? (Controle "+edpid+")"))
			location.href='academico.php?modulo=principal/listareditais&acao=C&evento=E&edpid='+edpid;
	}
	function submeter(evento, indid){		
			document.getElementById('evento').value = evento;		
			document.formulario.submit();
	}

</script>