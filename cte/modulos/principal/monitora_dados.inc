<?php

include_once( APPRAIZ . "cte/classes/EntidadeExecucaoSubacao.class.inc" );
include_once( APPRAIZ . "cte/classes/Polo.class.inc" );
include_once( APPRAIZ . "cte/classes/SituacaoCursista.class.inc" );
include_once( APPRAIZ . "includes/classes/fileSimec.class.inc");
//include_once( APPRAIZ . "includes/classes/modelo/entidade/Entidade.class.inc" );

if($_POST['verificacpf']){
	
	$eescpf = str_replace('-', '', str_replace('.', '', $_POST['eescpf']));
	
	$sql = "select 
				ees.eescpf				
			from cte.entidadeexecucaosubacao ees 
			inner join cte.execucaosubacao exe on exe.exeid = ees.exeid 
			inner join cte.periodoreferencia per on per.perid = exe.perid
			inner join cte.monitoramentosubacao mnt on mnt.mntid = exe.mntid
			inner join cte.subacaoindicador sba on mnt.sbaid = sba.sbaid
			where exe.exeatual = 't'
			and exe.mntid = {$_POST['mntid']}
			and ees.eescpf = '{$eescpf}'";
	
	echo $db->pegaUm($sql);	
	die();
}

if($_GET['download'] == 'S'){
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
    exit;
}

define("EST_NAOINICIADA", 1);
define("EST_ANDAMENTO", 2);
define("EST_SUSPENSO", 3);
define("EST_CANCELADO", 4);
define("EST_CONCLUIDO", 5);

$arUnidades = cte_recuperarArUnidadesExigemCPF();
					 
$arUnidadesSemPolo = array( CTE_UNIDADE_MEDIDA_CONSELHEIRO_MUN,
					 		CTE_UNIDADE_MEDIDA_CONSELHEIRO_EST,
						 	CTE_UNIDADE_MEDIDA_TECNICO_MUN,
						 	CTE_UNIDADE_MEDIDA_TECNICO_EST,
						  );

if(!$_SESSION['inuid'])
{	
	$html = "<script>
		alert('A sessão expirou, tente novamente.');
		window.opener.location.reload();
		window.close();
	</script>";
	die($html);	
} 

/********************************************************
 * FUNÇÃO PARA NAVEGAÇÃO ENTRE AS SUBAÇÕES
 */						  
$itrid = cte_pegarItrid($_SESSION['inuid']);

$inuid = $_SESSION['inuid'];

// Selecionando as subações cadastradas para monitoramento
$sql = "SELECT  DISTINCT			
			max(  mnt.mntid ) as mntid , 
			d.dimcod, 
			d.dimid, 
			d.dimdsc, 
			a.ardcod, 
			a.ardid, 
			a.arddsc, 
			i.indcod, 
			i.indid, 
			i.inddsc, 
			aim.aciid as acaomunicipalid, 
			aim.acidsc as acaomunicipal,
			saim.sbaid as subacaomunicipalid, 
			saim.sbaordem, 
			saim.sbadsc as subacaomunicipal, 
			saim.sbaordem			    
		  FROM cte.indicador i 
			INNER JOIN cte.areadimensao a on a.ardid = i.ardid 
			INNER JOIN cte.dimensao d on d.dimid = a.dimid 
			INNER  JOIN cte.pontuacao p on p.indid = i.indid 
				AND p.inuid = ".$inuid." 
				AND p.ptostatus = 'A' 
			INNER  JOIN cte.criterio c on c.crtid = p.crtid
			INNER  JOIN cte.acaoindicador aim on aim.ptoid = p.ptoid and aim.acilocalizador = 'M' 
			INNER  JOIN cte.subacaoindicador saim on saim.aciid = aim.aciid 
			INNER  JOIN cte.subacaoparecertecnico spt on saim.sbaid = spt.sbaid
			INNER JOIN cte.proposicaosubacao ps ON ps.ppsid = saim.ppsid
				LEFT JOIN cte.monitoramentosubacao AS mnt ON mnt.sbaid = saim.sbaid 
				LEFT JOIN cte.execucaosubacao AS exs ON exs.mntid = mnt.mntid AND exs.exeatual = TRUE
				LEFT JOIN cte.estadosubacao AS ess ON ess.estid = exs.estid
				LEFT JOIN seguranca.usuario AS usu ON exs.usucpf = usu.usucpf
					INNER JOIN cte.instrumentounidade AS ins ON ins.inuid = p.inuid 
					INNER JOIN workflow.documento AS doc ON doc.docid = ins.docid 
					INNER JOIN workflow.estadodocumento AS est ON est.esdid = doc.esdid 
		WHERE mnt.mntstatus = 'A' 
		AND i.indstatus = 'A' 
		AND a.ardstatus = 'A' 
		AND d.dimstatus = 'A' 
		AND d.itrid = ".$itrid."
		AND saim.frmid IN ( 1, 2, 4, 5, 6, 7, 8, 9, 10 )
		AND spt.ssuid = 3 
		AND ppsindcobuni = false
		AND est.esdid in ( 14, 15, 11, 13)
		
		GROUP BY			
			ess.estdsc,   
			mnt.mntid, 
			d.dimcod, 
			d.dimid, 
			d.dimdsc, 
			a.ardcod, 
			a.ardid, 
			a.arddsc, 
			i.indcod, 
			i.indid, 
			i.inddsc,
			acaomunicipalid,
			acaomunicipal,
			subacaomunicipalid,
			saim.sbaordem,
			subacaomunicipal,
			saim.sbaordem				
		ORDER BY d.dimcod, a.ardcod, i.indcod,  saim.sbaordem, saim.sbadsc			
		";

$subAcoes = (array) $db->carregar($sql);

$mntidAnterior = false;
$mntidProximo  = false;

while (list($k, $subacao) = each($subAcoes)) {
    if ($subacao["mntid"] == (integer) $_REQUEST['mntid']) {

        if (array_key_exists($k - 1, $subAcoes)) {
            $mntidAnterior = $subAcoes[$k - 1]["mntid"];
        }

        if (array_key_exists($k + 1, $subAcoes)) {
            $mntidProximo  = $subAcoes[$k + 1]["mntid"];            
        }

        break;
    }
}
						  
/********************************************************
 *  FUNÇÕES UTILIZADAS NO CONTROLE DO BD DAS DETERMINADAS ABAS 
 */
$muncod = cte_pegarMuncod($_SESSION['inuid']);

if( $_REQUEST["req"] == 'montarCombos' ){

	$i = $_REQUEST["qtd"];
	
	$arEntid = recuperarEscolasPorMuncod( $muncod );
	$indiceEntid = count($arEntid);
	$arEntid[$indiceEntid]["inep"] = '99007410';
	$arEntid[$indiceEntid]["codigo"] = '291572';
	$arEntid[$indiceEntid]["descricao"] = 'SECRETARIA MUNICIPAL DE EDUCAÇÃO';

	$obSituacaoCursista = new SituacaoCursista();
	$arSituacaoCursista = $obSituacaoCursista->recuperarTodos( 'scuid as codigo, scudsc as descricao' );
	
	$stEscolas = "";
	
	if(!$_POST['unidadeSemPolo'])
	{		
		$obPolo = new Polo();
		$arPolo = $obPolo->recuperarTodos( 'polid as codigo, polnome as descricao', array( "muncod = '$muncod'" ) );
		
		$stEscolas .= "
						var comboEscola = document.createElement( 'select' );
						comboEscola.className = 'comboEstiloEscola';
						
						var option = document.createElement( 'option' );
						option.setAttribute( 'value', '' );
						option.innerHTML = 'Selecione';
						comboEscola.appendChild( option );
						
						var comboPolo = document.createElement( 'select' );
						comboPolo.className = 'comboEstilo';
						
						var option = document.createElement( 'option' );
						option.setAttribute( 'value', '' );
						option.innerHTML = 'Selecione';
						comboPolo.appendChild( option );
						
						var comboSituacaoCursista = document.createElement( 'select' );
						comboSituacaoCursista.className = 'comboEstiloSituacao';
						
						var option = document.createElement( 'option' );
						option.setAttribute( 'value', '' );
						option.innerHTML = 'Selecione';
						comboSituacaoCursista.appendChild( option );
					 ";
						
	
		foreach( $arEntid as $indice => $arEntidade ){
						
			$stEscolas .= "
							var option = document.createElement( 'option' );
							option.setAttribute( 'value', '".$arEntidade["codigo"]."' );
							option.setAttribute( 'class', 'estiloEscola' );
							option.setAttribute( 'title', '".$arEntidade["descricao"]."' );
							option.innerHTML = '".$arEntidade["inep"]." - ".$arEntidade["descricao"]."';
							comboEscola.appendChild( option );
						  ";
		}
		
		foreach( $arPolo as $arDadosPolo ){
			$stEscolas .= "
							var option = document.createElement( 'option' );
							option.setAttribute( 'value', '".$arDadosPolo["codigo"]."' );
							option.innerHTML = '".iconv( "ISO-8859-1", "UTF-8", $arDadosPolo["descricao"] )."';
							comboPolo.appendChild( option ); 
						  ";
		}
		foreach( $arSituacaoCursista as $arDadosSituacaoCursista ){
			$stEscolas .= "
							var option = document.createElement( 'option' );
							option.setAttribute( 'value', '".$arDadosSituacaoCursista["codigo"]."' );
							option.innerHTML = '".$arDadosSituacaoCursista["descricao"]."';
							comboSituacaoCursista.appendChild( option ); 
						  ";
		}
		
		$stEscolas .= " 
						var imgObrigatorio = document.createElement( 'img' );
						imgObrigatorio.setAttribute( 'src', '../imagens/obrig.gif' );
						imgObrigatorio.setAttribute( 'title', 'Indica campo obrigatório.' );
	
						celula = linha.insertCell( 3 );
						celula.appendChild( comboPolo );
						
						celula = linha.insertCell( 4 );
						celula.appendChild( comboEscola );
						celula.appendChild( imgObrigatorio );
						
						celula = linha.insertCell( 5 );
						celula.appendChild( comboSituacaoCursista );
						celula.appendChild( imgObrigatorio );
					 ";
		
	} else {
		
		$stEscolas .= " var comboEscola = document.createElement( 'select' );
						comboEscola.className = 'comboEstiloEscola';
						
						var option = document.createElement( 'option' );
						option.setAttribute( 'value', '' );
						option.innerHTML = 'Selecione';
						comboEscola.appendChild( option );
						 
						var comboPolo = document.createElement( 'select' );
						comboPolo.className = 'comboEstilo';
						
						var option = document.createElement( 'option' );
						option.setAttribute( 'value', '' );
						option.innerHTML = 'Selecione';
						comboPolo.appendChild( option ); 
						 
						var comboSituacaoCursista = document.createElement( 'select' );
						comboSituacaoCursista.className = 'comboEstiloSituacao';
						
						var option = document.createElement( 'option' );
						option.setAttribute( 'value', '' );
						option.innerHTML = 'Selecione';
						comboSituacaoCursista.appendChild( option ); 
					";
		
		foreach( $arSituacaoCursista as $arDadosSituacaoCursista ){
			$stEscolas .= "					
							var option = document.createElement( 'option' );
							option.setAttribute( 'value', '".$arDadosSituacaoCursista["codigo"]."' );
							option.innerHTML = '".$arDadosSituacaoCursista["descricao"]."';
							comboSituacaoCursista.appendChild( option ); 
						  ";
		}
		
		$stEscolas .= " 
						var imgObrigatorio = document.createElement( 'img' );
						imgObrigatorio.setAttribute( 'src', '../imagens/obrig.gif' );
						imgObrigatorio.setAttribute( 'title', 'Indica campo obrigatório.' );
	
						celula = linha.insertCell( 3 );
						celula.appendChild( comboSituacaoCursista );
						celula.appendChild( imgObrigatorio );
					 ";
	}	

	echo $stEscolas;
	die();
}
					 
require  ( APPRAIZ. "www/includes/webservice/cpf.php" );

function carregarmotivosmonitoramento($dados) {
	  
	global $db;
	 
	if($dados['exeid']) {
		$complemento = ", (SELECT moddsc FROM cte.motivoexecucaosubacao mes WHERE mes.momid = mom.momid AND mes.exeid='".$dados['exeid']."') as moddsc, (SELECT moeid FROM cte.motivoexecucaosubacao mes WHERE mes.momid = mom.momid AND mes.exeid='".$dados['exeid']."') as moeid ";	
	}
	$sql = "SELECT mom.momid, mom.momdsc ".$complemento." 
			FROM cte.monitoramentosubacao mnt 
		  		LEFT JOIN cte.subacaoindicador sba ON mnt.sbaid = sba.sbaid 
		  		LEFT JOIN cte.motivomonitoramento mom ON mom.ppsid = sba.ppsid 
		  	WHERE mnt.mntid='".$dados['mntid']."' 
		  	AND mom.momid IS NOT NULL ORDER BY momdsc ASC";
	$motivos = $db->carregar($sql);
	echo "<table class='listagem' width='100%' cellspacing='1' cellpadding='3' align='center'>";
	$boOutros = false;
	if($motivos[0]) {
		echo "<thead><tr>";
		echo "<td style='width:15px;text-align:center;'>&nbsp;</td>";
		echo "<td align='center'><b>Motivos</b></td>";
		echo "<td align='center'><b>Observações</b></td>";
		echo "</tr></thead>";
		
		foreach($motivos as $motivo) {
			echo "<tr>";
			echo "<td style='width:15px;text-align:center;'><input type='checkbox' name='momid[]' value='".$motivo['momid']."' ".(($motivo['moeid'])?"checked":"")."></td>";
			echo "<td>".$motivo['momdsc']."</td>";
			echo "<td><input type='text' style='text-align: left;' onblur='MouseBlur(this);' onmouseout='MouseOut(this);' onfocus='this.select();' onmouseover='MouseOver(this);' class='normal' maxlength='250' size='50' name='momdsc[".$motivo['momid']."]' id='momdsc_".$motivo['momid']."' value='".$motivo['moddsc']."'/></td>";
			if( $motivo['momdsc'] == "Outros" ){
				$boOutros = true;
				$boOutrosVazio = true;
			} 
			if( $boOutros ){  
				echo "<input type='hidden' value='". $motivo['momid'] ."' name='momidoutros' id='momidoutros'>";
			}
			unset($boOutros);
			echo "</tr>";
		}
	} else {
		echo "<tr>";
		echo "<td colspan='3' class='SubTituloCentro'>Não existem motivos cadastrados</td>";
		echo "</tr>";
	}
	
	if( !isset($boOutrosVazio) ){  
		echo "<input type='hidden' value='' name='momidoutros' id='momidoutros' >";				
	}
		
	echo "</table>";
	exit;
}

function removeranexo($dados) {
	global $db;
	$sql = "DELETE FROM cte.versaoanexomonitoramento WHERE aneid='".$dados['aneid']."'";
	$db->executar($sql);
	$sql = "DELETE FROM cte.anexomonitoramento WHERE aneid='".$dados['aneid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Anexo removido com sucesso.');
			window.location='?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid']."';</script>";
	exit;
}
function removerexecucaosubacao($dados) {
	global $db;
	$sql = "DELETE FROM cte.execucaosubacao WHERE exeid='".$dados['exeid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Execução removido com sucesso.');
			window.location='?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid']."';</script>";
	exit;
}
function inseriranexo($dados) {
	global $db;	
	
	if(!$_SESSION['cte_var']['mntid']){
		
		$html = "<script>
					alert('A variável da sessão expirou, tente novamente.');
					window.opener.location.reload();
					window.close();
				</script>";
		die($html);
			
	}
	
	if( $dados['tamid'] ){
		$sql = "INSERT INTO cte.anexomonitoramento(
	            anedsc, anestatus, tamid, usucpf, mntid, anedtinclusao)
	    		VALUES ('".$dados['anedsc']."', 'A', '".$dados['tamid']."', '".$_SESSION['usucpf']."', '".$_SESSION['cte_var']['mntid']."', NOW()) RETURNING aneid;";
		$dados['aneid'] = $db->pegaUm($sql);
		inserirversaoanexo($dados);
	}else{
		return false;
	}
}
function inserirversaoanexo($dados) {
	global $db;
	$arqid = inserirarquivo();
	$sql = "INSERT INTO cte.versaoanexomonitoramento(
            aneid, arqid)
    		VALUES ('".$dados['aneid']."', '".$arqid."');";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Arquivo inserido com sucesso.');
			window.location='?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid']."';
		  </script>";
	exit;
}

function inserirarquivo() {
	global $db;
	// obtém o arquivo
	$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		echo "<script>alert('Erro no upload. Entre em contato com a equipe técnica');</script>";
		exit;
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
	values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".$dados["arqdescricao"]."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	if(!is_dir('../../arquivos/cte/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/cte/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			$original_y = $height;
			// se a largura for maior que altura
			if($original_x == 0){
				$original_x = 1;
			}
			
			if($original_y == 0){
				$original_y = 1;
			}
			
			if($original_x > $original_y) {
  	 			$porcentagem = (100 * 640) / $original_x;      
			}else {
   				$porcentagem = (100 * 480) / $original_y;  
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if (!move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
				exit;
			}
	}
	return $arqid;
}

function removerobservacao($dados) {
	global $db;
	$sql = "UPDATE cte.observacaomonitoramento SET obsstatus='I' WHERE obsid='".$dados['obsid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Observação removida com sucesso.');
			window.location='?modulo=principal/monitora_dados&acao=A&_page=observacoes';
		  </script>";
	exit;
}

function inserirobservacao($dados) {
	global $db;
	$sql = "INSERT INTO cte.observacaomonitoramento(obsdsc, obsstatus, usucpf, mntid, obsdtinclusao)
    		VALUES ('".$dados['obsdsc']."', 'A', '".$_SESSION['usucpf']."', '".$_SESSION['cte_var']['mntid']."', NOW());";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Observação inserida com sucesso.');
			window.location='?modulo=principal/monitora_dados&acao=A&_page=observacoes';
		  </script>";
	exit;
	
}
function removerrestricaoexecucao($dados) {
	global $db;
	$sql = "DELETE FROM cte.restricaoexecucao WHERE resid='".$dados['resid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Restrição removida com sucesso.');
			window.location='?modulo=principal/monitora_dados&acao=A&_page=restricao';
		  </script>";
	exit;	

}
function atualizarrestricaoexecucao($dados) {
	global $db;
	$sql = "UPDATE cte.restricaoexecucao
   	 		SET ".(($dados['ressuperada']=="sim")?"usucpfsuperacao='".$_SESSION['usucpf']."',resdtsuperacao=NOW(),":"")."
   	 		resprovidencia='".$dados['resprovidencia']."', 
     		ressuperada=".(($dados['ressuperada']=="sim")?"TRUE":"FALSE")." 
 	 		WHERE resid='".$dados['resid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Restrição atualizada com sucesso.');
			window.location='?modulo=principal/monitora_dados&acao=A&_page=restricao';
		  </script>";
	exit;
	
}
function inserirrestricaoexecucao($dados) {
	global $db;
	
	if(!$_SESSION['cte_var']['mntid'])
	{	
		$html = "<script>
			alert('A sessão expirou, tente novamente.');
			window.opener.location.reload();
			window.close();
		</script>";
		die($html);	
	}
 
	$sql = "INSERT INTO cte.restricaoexecucao(usucpf, 
											  mntid, 
											  resdsc, 
											  resprovidencia, 
            								  resdtinclusao,
            								  trscod)
    		VALUES ('".$_SESSION['usucpf']."', 
    				'".$_SESSION['cte_var']['mntid']."', 
    				'".$dados['resdsc']."', 
    				'".$dados['resprovidencia']."', 
            		NOW(),
            		'".$_POST['trscod_disable']."');";
	
	if(!$_SESSION['cte_var']['mntid'])
	{
		echo "<script>
			alert('A variável da sessão não existe, tente de novo');
			window.close();
		</script>";
	}	
	
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Restrição inserida com sucesso.');
			window.location='?modulo=principal/monitora_dados&acao=A&_page=restricao';
		  </script>";
	exit;
}
function inserirexecucaosubacao($dados) {
	
	global $db, $arUnidades;	
	
	if($_SESSION['cte_var']['mntid'] && $dados['estid']){
	
		switch($dados['estid']) {
			case EST_NAOINICIADA:
			case EST_SUSPENSO:
			case EST_CANCELADO:
	
			if($_SESSION['cte_var']['mntid'])
			{
				$db->executar("UPDATE cte.execucaosubacao SET exeatual = FALSE WHERE mntid = '".$_SESSION['cte_var']['mntid']."' AND perid = '".$dados['perid']."'");
			}
			$sql = "INSERT INTO cte.execucaosubacao( mntid, usucpf, estid, exedtexecucao, exeqtd, exeobs, exeatual, perid,  exedtinclusao, corcod)
	    			VALUES ('".$_SESSION['cte_var']['mntid']."', 
	    					'".$_SESSION['usucpf']."', 
	    					'".$dados['estid']."', 
	    					NULL, 
		    				NULL, 
	    					'".$dados['exeobs']."',
	    					TRUE,
	    					'".$dados['perid']."',
	    					NOW(),
	    					0) RETURNING exeid;";
			
			$exeid = $db->pegaUm($sql);
			
			$sql = "SELECT inumonitoratecnico FROM cte.instrumentounidade WHERE inuid = {$_SESSION['inuid']}";
			$inumonitoratecnico = $db->pegaUm($sql);
						
			if($inumonitoratecnico == 'f'){
				
				$sql = "UPDATE cte.instrumentounidade SET inumonitoratecnico = true WHERE inuid = {$_SESSION['inuid']}";
				$db->executar($sql);
			}						
			
			if($dados['momid']) {
				 
				foreach($dados['momid'] as $momid) {
					$sql = "INSERT INTO cte.motivoexecucaosubacao(
	            			momid, exeid, moddsc)
	    					VALUES ('".$momid."', '".$exeid."', '".$dados['momdsc'][$momid]."');";
					 
					$db->executar($sql);
				}
				 
			}
			$db->commit();
			break;
			default:
			if($_SESSION['cte_var']['mntid'])
			{
				$db->executar("UPDATE cte.execucaosubacao SET exeatual = FALSE WHERE mntid = '".$_SESSION['cte_var']['mntid']."' AND perid = '".$dados['perid']."'");
			}		
			if( trim( $dados['corcod'] ) != ''){
				$sqlCorValue = ", ".$dados['corcod'];
				$sqlCorSelect = ", corcod"; 
			}else{
				$sqlCorValue ="";
				$sqlCorSelect = ""; 
			}
			if( trim( $dados['exeqtd'] ) != ''){ 
				$sqlQtdValue = "'".$dados['exeqtd']."', ";
				$sqlQtdSelect = "exeqtd ,";
			}else{ 
				$sqlQtdValue = "";
				$sqlQtdSelect = "";
			}
			
			$sql = "INSERT INTO cte.execucaosubacao( mntid, usucpf, estid, exedtexecucao, $sqlQtdSelect exeobs, exeatual, perid,  exedtinclusao $sqlCorSelect)
	    			VALUES ('".$_SESSION['cte_var']['mntid']."',
	    					'".$_SESSION['usucpf']."', 
	    					'".$dados['estid']."', 
	    					'".$dados['exedtexecucao_ano'].$dados['exedtexecucao_mes']."', 
		    				$sqlQtdValue
	    					'".$dados['exeobs']."', 
	    					TRUE,
	    					'".$dados['perid']."',
	    					NOW()
							$sqlCorValue );";
			$db->executar($sql);
		}
	} else {
		$html = "<script>
			alert('A varivável sessão expirou, tente novamente.');
			window.opener.location.reload();
			window.close();
		</script>";
		die($html);			
	}
	
	$db->commit(); 

	if($_REQUEST["undid"] == ''){
		$peridIndice = $_REQUEST['perid'] - 1;
		$sql = "
				select undid from cte.execucaosubacao exe
				inner join cte.monitoramentosubacao mnt on exe.mntid = mnt.mntid
				inner join cte.subacaoindicador sba on sba.sbaid = mnt.sbaid
				where mnt.mntid = ".$_SESSION['cte_var']['mntid']." 
				and estid in (1, 2, 3) 
				and exeatual = true
				and perid = '".$peridIndice."' 	
			   ";		
		
		$_REQUEST["undid"] = $db->pegaUm($sql);
	}
	
	if( in_array( $_REQUEST["undid"], $arUnidades ) ){
	 
		if( is_array( $_REQUEST["arees"] ) ){
	
			$sql = "SELECT exeid 
					FROM cte.execucaosubacao 
					WHERE exeatual = TRUE 
					AND mntid='".$_SESSION['cte_var']['mntid']."' 
					AND perid = '".$dados['perid']."'";
			
			$exeid = $db->pegaUm($sql);			
			
			foreach( $_REQUEST["arees"] as $count ){
				
				$obEntidadeExecucaoSubacao = new EntidadeExecucaoSubacao();
				
				$eescpf = "eescpf_$count";
				$polid = "polid_$count";
				
				$eesentescola = "eesentescola_$count";
									
				$obEntidadeExecucaoSubacao->exeid = $exeid;
				$obEntidadeExecucaoSubacao->eescpf = ereg_replace("[^0-9]", "", $_REQUEST["eescpf_$count"] );
				$obEntidadeExecucaoSubacao->eesnome = $_REQUEST["eesnome_$count"]; //"teste$count";
				 
				if( $_REQUEST["eesentescola_$count"] ){
					$obEntidadeExecucaoSubacao->eesentescola = $_REQUEST["eesentescola_$count"];
				}
				
				if( $_REQUEST["polid_$count"] ){
					$obEntidadeExecucaoSubacao->polid = $_REQUEST["polid_$count"];
				}
				
				if($_REQUEST["scuid_$count"])
				{
					$obEntidadeExecucaoSubacao->scuid = $_REQUEST["scuid_$count"];
				}
				
				$obEntidadeExecucaoSubacao->salvar();
				$obEntidadeExecucaoSubacao->commit();

				unset( $obEntidadeExecucaoSubacao );				
			} 
		}
	}
	elseif( $_REQUEST["undid"] == CTE_UNIDADE_MEDIDA_UNIDADES_ESCOLARES_MUN || $_REQUEST["undid"] == CTE_UNIDADE_MEDIDA_UNIDADES_ESCOLARES_EST  ){
		
		if( is_array( $_REQUEST["arees"] ) ){
	
			$sql = "SELECT exeid 
					FROM cte.execucaosubacao 
					WHERE exeatual = TRUE 
					AND mntid='".$_SESSION['cte_var']['mntid']."' 
					AND perid = '".$dados['perid']."'";
			
			$exeid = $db->pegaUm($sql);
			
			foreach( $_REQUEST["arees"] as $count ){
				
				$obEntidadeExecucaoSubacao = new EntidadeExecucaoSubacao();
				
				$eesentescola = "eesentescola_$count";
				
				if( $_REQUEST["eesentescola_$count"] ){
					
					$obEntidadeExecucaoSubacao->exeid = $exeid;
					$obEntidadeExecucaoSubacao->eesentescola = $_REQUEST["eesentescola_$count"];
					$obEntidadeExecucaoSubacao->eesentqtd = $_REQUEST["eesentqtd_$count"] ? $_REQUEST["eesentqtd_$count"] : 0;
					
					$obEntidadeExecucaoSubacao->salvar();
					$obEntidadeExecucaoSubacao->commit();
	
					unset( $obEntidadeExecucaoSubacao );
				}
			}
		}		
	}
	
	if($_REQUEST['mntidT']){
		echo "<body>
			  <form id='forminclusao' method='post' action='?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_REQUEST['mntidT']."&perano=".$dados['exedtexecucao_ano']."'>
			  <input type='hidden' name='perid' value='".$dados['perid']."'>
			  </form>
			  </body>";
		echo "<script>
			  alert('Execução inserida com sucesso.');		  
			  window.location.href;		  
			  document.getElementById('forminclusao').submit();		  
			  </script>";
		exit;
	} else {	
		echo "<body>
			  <form id='forminclusao' method='post' action='?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid']."&perano=".$dados['exedtexecucao_ano']."'>
			  <input type='hidden' name='perid' value='".$dados['perid']."'>
			  </form>
			  </body>";
		echo "<script>
			  alert('Execução inserida com sucesso.');
			  window.opener.location.reload();
			  window.location.href;		  
			  document.getElementById('forminclusao').submit();		  
			  </script>";
		exit;
	}
	
}
/********************************************************
 * FIM - FUNÇÕES UTILIZADAS NO CONTROLE DO BD DAS DETERMINADAS ABAS 
 */
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
}
?>
<html>
<head>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<style type="text/css">
		.comboEstilo{
			border-right: #888888 1px solid;
		    border-top: #888888 1px solid;
		    font-size: 10px;
		    vertical-align: middle;
		    border-left: #000000 3px solid;
		    border-bottom: #888888 1px solid;
		    font-family: verdana;
		    background-color: #ffffff;
		    width:130px;
		}
		
		.comboEstiloSituacao{
			border-right: #888888 1px solid;
		    border-top: #888888 1px solid;
		    font-size: 10px;
		    vertical-align: middle;
		    border-left: #000000 3px solid;
		    border-bottom: #888888 1px solid;
		    font-family: verdana;
		    background-color: #ffffff;
		    width:150px;
		}
		
		.comboEstiloEscola{
			border-right: #888888 1px solid;
		    border-top: #888888 1px solid;
		    font-size: 10px;
		    vertical-align: middle;
		    border-left: #000000 3px solid;
		    border-bottom: #888888 1px solid;
		    font-family: verdana;
		    background-color: #ffffff;
		    width:240px;
		}		
				
		option {
			width: 260px;						
		}				
	</style>
	<script language="JavaScript">
		function fecharPopup(){
			window.opener.location.reload();
			window.close();			
		}
		function alterarTipoUnidadeMedida( tipo ){
			document.getElementById('tipo_unidade_medida').value = tipo;
		}
		function navegarForm(anoSub, vlrProximaSubacao){
			if (vlrProximaSubacao != '') {
                window.location.href = '/cte/cte.php?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=' + vlrProximaSubacao + '&perano=' + anoSub; 
            } else {
                window.opener.location.reload();
                window.opener.focus();
                window.close();
            }
		}
		
		jQuery.noConflict();
		
		jQuery(document).ready(function(){
			jQuery('.eescpf').live('blur', function(){

				var eesid = this.id;
				var arDado = eesid.split('_');
				
				jQuery.ajax({
					type	: "POST",
					url		: document.location.href,
					data	: "verificacpf=true&eescpf="+this.value+"&mntid="+<?php echo $_GET['mntid'] ?>,
					async	: false,
					success	: function(e){

						if(e != ''){
							jQuery('#'+eesid).val(null);
							jQuery('#eesnome_'+arDado[1]).val(null);
							alert("Este CPF já existe para este monitoramento.");
							return false;	
						}
					}
				});
			});
		});
	</script> 	
</head>
<body>
<script language="JavaScript" src="../includes/mmenu.js"></script>
<?php
if(!$_REQUEST['_page']) {
	$_REQUEST['_page'] = 'execucao';
}
if($_REQUEST['mntid']) {
	$_SESSION['cte_var']['mntid'] = $_REQUEST['mntid'];
}
// Criando as abas de navegação dentro de uma instância de monitoramento
$abas_monitoramento = array(0 => array("id" => 1, "descricao" => "Execução", "link" => "/cte/cte.php?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid'].(($_REQUEST['perano'])?"&perano=".$_REQUEST['perano']:"")),
							1 => array("id" => 2, "descricao" => "Restrição", "link" => "/cte/cte.php?modulo=principal/monitora_dados&acao=A&_page=restricao"),
						   );
						   
// Fim da criação de abas
echo montarAbasArray($abas_monitoramento, $_SERVER['REQUEST_URI'] );

$titulo_modulo = "PAR - Plano de Metas";
monta_titulo( $titulo_modulo,'Monitoramento do PAR');

// Buscando subação vinculada ao monitoramento
if($_SESSION['cte_var']['mntid'])
{
$subac = $db->pegaLinha("SELECT sba.sbaordem, sba.sbaid, sba.sbadsc, sba.aciid, sbastgmpl, sba.sbaid, sba.prgid FROM cte.monitoramentosubacao mnt
						 LEFT JOIN cte.subacaoindicador sba ON sba.sbaid = mnt.sbaid 
						 WHERE mnt.mntid='".$_SESSION['cte_var']['mntid']."'");
} else {
	alert("Tente de novo, a variável da sessão expirou!");
	echo "<script>
	window.close();	
	</script>";
	exit;
}

if(!$subac['aciid']){
	alert("Tente de novo, a variável da sessão expirou!");
	echo "<script>
	window.close();	
	</script>";
	exit;
}

// Verificando se a data atual permite cadastramento
$permissaoperiodocadastramento = $db->carregar("SELECT * FROM cte.periodoreferencia WHERE NOW() >= perdtiniciocad AND NOW() <= perdtterminocad");

if(!$permissaoperiodocadastramento[0]) {
	unset($permissaoperiodocadastramento);
}
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td colspan="2">
			<h3 title="Unidade da Federação">
				<?php
				$descricaom = cte_pegarMundescricao($muncod);
				echo $descricaom;
				?>
			</h3>
		</td>
	</tr>
	<?php	
	$sql = "SELECT
	            dim.dimid,
	            dim.dimcod,
	            dim.dimdsc,
	            are.ardid,
	            are.ardcod,
	            are.arddsc,
	            ind.indid,
	            ind.indcod,
	            ind.inddsc,
	            aci.acidsc,
	            aci.acirpns,
	            to_char(aci.acidtinicial,'dd/mm/YYYY') as acidtinicial,
	            to_char(aci.acidtfinal,'dd/mm/YYYY') as acidtfinal,
	            aci.acirstd
	        FROM cte.dimensao dim
	        LEFT OUTER JOIN cte.areadimensao are ON are.dimid=dim.dimid 
	        LEFT OUTER JOIN cte.indicador ind ON ind.ardid=are.ardid
	        LEFT OUTER JOIN cte.pontuacao pto ON pto.indid=ind.indid and ptostatus = 'A'
	        LEFT OUTER JOIN cte.acaoindicador aci ON aci.ptoid = pto.ptoid  
	        WHERE aci.aciid='".$subac['aciid']."'";
	
	$acaind = $db->pegaLinha($sql);
	
	if($acaind) {
	?>
	<tr align="left">
		<td>
			<input onclick="return navegarForm('<?=$_REQUEST['perano']?>', '<?=$mntidAnterior?>');" type="button" name="btnAnterior" value="Anterior" id="btnAnterior" <?php echo !$mntidAnterior ? 'disabled="disabled"' : ''; ?>/>
		</td>
		<td align="right">
			<input onclick="return navegarForm('<?=$_REQUEST['perano']?>', '<?=$mntidProximo?>');" type="button" name="btnProximo" value="Próxima" id="btnProximo" <?php echo !$mntidProximo ? 'disabled="disabled"' : ''; ?>/>
		</td>
	</tr>
	<tr><td class="SubTituloDireita">Dimensão :</td><td><? echo $acaind['dimcod'].". ".$acaind['dimdsc']; ?></td></tr>
	<tr><td class="SubTituloDireita">Área :</td><td><? echo $acaind['ardcod'].". ".$acaind['arddsc']; ?></td></tr>
	<tr><td class="SubTituloDireita">Indicador :</td><td><? echo $acaind['indcod'].". ".$acaind['inddsc']; ?></td></tr>
	<tr><td class="SubTituloDireita">Programa :</td><td><? echo $db->pegaUm("SELECT prgdsc FROM cte.programa WHERE prgid='".$subac['prgid']."'"); ?></td></tr>
	<tr><td class="SubTituloDireita">Ação :</td><td><? echo $acaind['acidsc']; ?></td></tr>
	<tr><td class="SubTituloDireita">Nome do Responsável :</td><td><? echo $acaind['acirpns']; ?></td></tr>
	<tr><td class="SubTituloDireita">Período Inicial :</td><td><? echo $acaind['acidtinicial']; ?></td></tr>
	<tr><td class="SubTituloDireita">Período Final :</td><td><? echo $acaind['acidtfinal']; ?></td></tr>
	<tr><td class="SubTituloDireita">Resultado Esperado :</td><td><? echo $acaind['acirstd']; ?></td></tr>
	<?php }	?>
	<tr><td class="SubTituloDireita">Descrição Subação :</td><td><? echo $acaind['dimcod'].".".$acaind['ardcod'].".".$acaind['indcod']." - ".$subac['sbaordem']." ".$subac['sbadsc']; ?></td></tr>
	<tr><td class="SubTituloDireita">Estratégia de Implementação :</td><td><? echo $subac['sbastgmpl']; ?></td></tr>
</table>
<br />
<?php
$peridTrata = $_REQUEST['perid'] ? $_REQUEST['perid'] : 0;
$totalPrevEscolas['total']	= "";
$totalExecutado['sum'] 		= "";
$totalPrevisto['sptunt'] 	= "";

if($_REQUEST['perano']){
	$sql = "SELECT sptano, sptunt FROM cte.subacaoparecertecnico WHERE sbaid = '".$subac['sbaid']."' AND sptano = '".$_REQUEST['perano']."'";
	$totalPrevisto = $db->pegaLinha($sql);
	
	
	
	$sql = "SELECT sum(exeqtd) FROM cte.execucaosubacao WHERE mntid = '".$_SESSION['cte_var']['mntid']."' AND exeatual = 't' AND substr(exedtexecucao, 0, 5) = '".$_REQUEST['perano']."' AND perid <> '".$peridTrata."'";
	$totalExecutado = $db->pegaLinha($sql);
}

$totalExecutado['sum'] 		= $totalExecutado['sum'] ? $totalExecutado['sum'] : 0;
$totalPrevisto['sptunt'] 	= $totalPrevisto['sptunt'] ? $totalPrevisto['sptunt'] : 0;

$qtdPrevista['previsto'] = "";
if($subac['sbaid']){
	$sql = "SELECT sum(sptunt) as previsto FROM cte.subacaoparecertecnico 
			WHERE sbaid = '".$subac['sbaid']."'";
	$qtdPrevista = $db->pegaLinha($sql);
	
	$sql = "SELECT
				--'' as qfaid,
				--e.entcodent,
				--e.entnome,
				--sum(coalesce(qfaqtd,0))
				count(*) as total
			FROM cte.qtdfisicoano q
			INNER JOIN entidade.entidade e on q.entid = e.entid
			WHERE q.sbaid  = ".$subac['sbaid']."
			GROUP BY q.qfaid, e.entcodent, e.entnome
			ORDER BY entnome";
	
	$qtdPrevista2 = $db->pegaLinha($sql);
	$qtdPrevista['previsto'] = $qtdPrevista['previsto'] ? $qtdPrevista['previsto'] : $qtdPrevista2['total'];
}

$qtdExecutada['executado'] = "";
if ($_SESSION['cte_var']['mntid']){
	$sql = "SELECT sum(coalesce(exeqtd, 0)) as executado FROM cte.execucaosubacao 
			WHERE mntid = ".$_SESSION['cte_var']['mntid']."
			AND exeatual = true";
	$qtdExecutada = $db->pegaLinha($sql);
	
	if($_REQUEST['perid']){
		$stWhereExe = "AND perid <> '".$_REQUEST['perid']."'"; 
	}
	
	$sql = "SELECT sum(coalesce(exeqtd, 0)) as executado FROM cte.execucaosubacao 
			WHERE mntid = ".$_SESSION['cte_var']['mntid']."
			AND exeatual = true
			$stWhereExe
			";
	$qtdExecutadaPeriodos = $db->pegaLinha($sql);
	
}
?>
<script>
<!--
function removeSubacao(){
	var mntid = <?=$_SESSION['cte_var']['mntid'];?>;
	var perano = <?=$_REQUEST['perano'] ? $_REQUEST['perano'] : date('Y');?>;
	<? if(isset($_REQUEST['perid'])) { ?>
		var perid = <?=$_REQUEST['perid'];?>;
		if(!confirm("Monitoramento fora do cronograma, deseja excluí-lo?")){	
			return false;
		} else {
			window.location.href = 'cte.php?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid='+mntid+'&perano='+perano+'&removeSubacao=true&perid='+perid;
		}
	<? } ?>
}

function getElementsByName_iefix(tag, name) 
{             
var elem = document.getElementsByTagName(tag);  
var arr = new Array();  
	for(i = 0,iarr = 0; i < elem.length; i++) {  
		att = elem[i].getAttribute("name");  
		if(att == name) 
		{  
			arr[iarr] = elem[i];  
			iarr++;  
		}  
	}  
	return arr;  
}
	
function ajaxatualizar(params,iddestinatario) {	
 
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				 
				if(iddestinatario) {
					document.getElementById(iddestinatario).innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				document.getElementById(iddestinatario).innerHTML = 'Carregando...';
			}
		});
}
function in_array(needle, haystack, argStrict) {
 
    // *     example 1: in_array('van', ['Kevin', 'van', 'Zonneveld']);
    // *     returns 1: true
    // *     example 2: in_array('vlado', {0: 'Kevin', vlado: 'van', 1: 'Zonneveld'});
    // *     returns 2: false
    // *     example 3: in_array(1, ['1', '2', '3']);
    // *     returns 3: true
    // *     example 3: in_array(1, ['1', '2', '3'], false);
    // *     returns 3: true
    // *     example 4: in_array(1, ['1', '2', '3'], true);
    // *     returns 4: false
 
    var key = '', strict = !!argStrict;
 
    if (strict) {
        for (key in haystack) {
            if (haystack[key] === needle) {
                return true;
            }
        }
    } else {
        for (key in haystack) {
            if (haystack[key] == needle) {
                return true;
            }
        }
    }
 
    return false;
}
function verificarmotivo(estid) {

	var tipoUnidadeMedida = document.getElementById('tipo_unidade_medida').value; 

	switch(estid) {
		case '<?php echo EST_NAOINICIADA; ?>':
		case '<?php echo EST_SUSPENSO; ?>':
		case '<?php echo EST_CANCELADO; ?>':
			if( tipoUnidadeMedida == "pessoa" ){
				document.getElementById('tr_pessoa').style.display = 'none';
			}
			else{
				if( tipoUnidadeMedida == "escolas" ){
					document.getElementById('tr_escolas').style.display = 'none';
				}
			}
			document.getElementById('tr_acumuladoate').style.display = 'none';
			document.getElementById('tr_quantidade').style.display = 'none';
	//		document.getElementById('tr_observacoes').style.display = 'none';
			
			document.getElementById('tr_situacao').style.display = 'none';
			
			if( estid == '<?php echo EST_NAOINICIADA; ?>' ){
				document.getElementById('tr_motivos').style.display = '';
				ajaxatualizar('requisicao=carregarmotivosmonitoramento&exeid=<? echo $execucao['exeid']; ?>','td_motivos');		
			}
			else{
				document.getElementById('tr_motivos').style.display = 'none';
			}
			break;
		default:
			if( tipoUnidadeMedida == "pessoa" ){
				document.getElementById('tr_pessoa').style.display = '';
			}
			else{
				if( tipoUnidadeMedida == "escolas" ){
					document.getElementById('tr_escolas').style.display = '';
				}
			}
			document.getElementById('tr_acumuladoate').style.display = '';
			document.getElementById('tr_quantidade').style.display = '';
	//		document.getElementById('tr_observacoes').style.display = '';
			 
			if( ( estid == <?=EST_CONCLUIDO?> ) && ( estid != <?=EST_ANDAMENTO?> ) ){
				document.getElementById('tr_situacao').style.display = 'none';
			}else{ 
				document.getElementById('tr_situacao').style.display = '';	
			}
			document.getElementById('tr_motivos').style.display = 'none';
	}

}

function ExcluirPorSubmit(formid, msg) {
	if(confirm(msg)) {
		document.getElementById(formid).submit();
	}
}
function Excluir(url, msg) {
	if(confirm(msg)) {
		window.location=url;
	}
}
function validardocumento() {

	if( document.getElementById( 'tamid' ).value == '' ){
		alert('"Tipo de documento" é obrigatório');
		return false;
	}
	
	if( document.getElementById( 'tamid' ).value == 21 && !document.getElementById( 'anedsc' ).value ){
		alert('"Descrição" é obrigatória');
		return false;
	}
	document.getElementById('btnsalvarDocumento').disabled=true;
	document.getElementById('formularioDocumento').submit();
}
function validarexecucao(navegarSub) {

	var arUnidades = new Array();
		arUnidades[0] = 			 '<?=CTE_UNIDADE_MEDIDA_SERVIDOR_MUN ?>'; 
		arUnidades[1] = 			 '<?=CTE_UNIDADE_MEDIDA_SERVIDOR_EST?>'; 
		arUnidades[2] = 			 '<?=CTE_UNIDADE_MEDIDA_PROFESSOR_MUN?>'; 
		arUnidades[3] = 			 '<?=CTE_UNIDADE_MEDIDA_PROFESSOR_EST?>';
		arUnidades[4] = 			 '<?=CTE_UNIDADE_MEDIDA_PROFESSOR_MULTIPLICADOR_MUN?>';
		arUnidades[5] = 			 '<?=CTE_UNIDADE_MEDIDA_PROFESSOR_MULTIPLICADOR_EST?>';
		arUnidades[6] = 			 '<?=CTE_UNIDADE_MEDIDA_PROFESSOR_CURSISTA_MUN?>';
		arUnidades[7] = 			 '<?=CTE_UNIDADE_MEDIDA_PROFESSOR_CURSISTA_EST?>';
		arUnidades[8] = 			 '<?=CTE_UNIDADE_MEDIDA_PROFESSOR_TUTOR_MUN?>';
		arUnidades[9] = 			 '<?=CTE_UNIDADE_MEDIDA_PROFESSOR_TUTOR_EST?>';
		arUnidades[10] = 			 '<?=CTE_UNIDADE_MEDIDA_DIRETOR_MUN?>';
		arUnidades[11] = 			 '<?=CTE_UNIDADE_MEDIDA_DIRETOR_EST?>';
		arUnidades[12] = 			 '<?=CTE_UNIDADE_MEDIDA_CONSELHEIRO_MUN?>';
		arUnidades[13] = 			 '<?=CTE_UNIDADE_MEDIDA_CONSELHEIRO_EST?>';
		arUnidades[14] = 			 '<?=CTE_UNIDADE_MEDIDA_TECNICO_MUN?>';
		arUnidades[15] = 			 '<?=CTE_UNIDADE_MEDIDA_TECNICO_EST?>';
		arUnidades[16] = 			 '<?=CTE_UNIDADE_MEDIDA_FUNCIONARIO_MUN?>'; 
		arUnidades[17] = 			 '<?=CTE_UNIDADE_MEDIDA_FUNCIONARIO_EST?>';
		arUnidades[18] = 			 '<?=CTE_UNIDADE_MEDIDA_SERVIDOR_SME?>';		
		arUnidades[19] = 			 '<?=CTE_UNIDADE_MEDIDA_CONSELHEIRO_MUN?>';
		arUnidades[20] = 			 '<?=CTE_UNIDADE_MEDIDA_CONSELHEIRO_EST?>'; 
		arUnidades[21] = 			 '<?=CTE_UNIDADE_MEDIDA_TECNICO_MUN?>';
		arUnidades[22] = 			 '<?=CTE_UNIDADE_MEDIDA_TECNICO_EST?>';
		arUnidades[23] = 			 '<?=CTE_UNIDADE_MEDIDA_REUNIOES?>';
		arUnidades[24] = 			 '<?=CTE_UNIDADE_MEDIDA_DOCUMENTOS?>'; 
		arUnidades[25] = 			 '<?=CTE_UNIDADE_MEDIDA_KIT_MATERIAL?>';
		arUnidades[26] = 			 '<?=CTE_UNIDADE_MEDIDA_DIAGNOSTICO?>';
	
	var totalPrevisto = <?=$qtdPrevista['previsto'] = $qtdPrevista['previsto'] ? $qtdPrevista['previsto'] : 0;?>;
	var exeqtdAnterior = <?=$qtdExecutadaPeriodos['executado'] = $qtdExecutadaPeriodos['executado'] ? $qtdExecutadaPeriodos['executado'] : 0;?>;
	var exeqtdAtual = document.getElementById("exeqtd").value;
	var totalExecutado = parseInt(exeqtdAnterior) + parseInt(exeqtdAtual);
	var situacao = document.getElementById('estid').value; 
	
	if(situacao == '2'){
		if(totalExecutado > totalPrevisto){
			if(!confirm("A quantidade total executada é maior que a prevista, deseja continuar?")){
			//if(!confirm("A quantidade total executada("+totalExecutado+") é maior que a prevista("+totalPrevisto+"), deseja continuar?")){
				return false;
			};
		}
	}
	if(situacao == '5'){
		if(totalExecutado > totalPrevisto){
			if(!confirm("A quantidade total executada é maior que a prevista, deseja concluir esta execução?")){		
				return false;
			};		
		}
		if(totalExecutado < totalPrevisto){
			if(!confirm("A quantidade total executada é menor que a prevista, deseja concluir esta execução?")){		
				return false;
			};		
		}	
	}
		
	if(document.formulario.elements['estid'].value == '') { 
		alert('"Estado da subação" é obrigatório');
		return false;
	}
	
	if(document.formulario.exeobs.value == '') {
		alert('"Observações" é obrigatório.');
		document.formulario.exeobs.focus();
		return false;
	}
	
	checkOutros = document.getElementsByName( 'momid[]' );
	dscOutros = document.getElementsByName( 'momid[]' );
	
	if(document.getElementsByName( 'momdsc_3756	' ).checked) { 
		alert('"Observação" é obrigatório');
		return false;
	}	
	 
	var nrEsdid = document.getElementById( 'estid' ).value;
	
	if( nrEsdid != '<?php echo EST_NAOINICIADA; ?>' && nrEsdid != '<?php echo EST_SUSPENSO; ?>' && nrEsdid != '<?php echo EST_CANCELADO; ?>' && nrEsdid != '<?php echo EST_CONCLUIDO; ?>' ) {
	
		if(document.formulario.elements['exeqtd'].value == '') {
			alert('"Quantidade" é obrigatório');
			return false;
		}
		if(document.formulario.elements['exedtexecucao_mes'].value == '') {
			alert('"Acumulado até" é obrigatório');
			document.formulario.elements['exedtexecucao_mes'].focus();
			return false;
		}
		/*		
		if(document.formulario.elements['exeobs'].value == '') {
			alert('"Observações" é obrigatório.');
			document.formulario.elements['exeobs'].focus();
			return false;
		}
		*/ 
		var exeqtd 	 = document.formulario.elements['exeqtd'];
		var tabela 	 = document.getElementById( "tabelaEscolas" );
		var undid 	 = document.getElementById( "undid" );
		var arCorcod = document.getElementsByName( "corcod" );
		
		var boCorMarcada = false;
		for( i=0; i<arCorcod.length; i++ ){
			if( arCorcod[i].checked ){
				boCorMarcada = true;
				break;
			}
		}
		if( !boCorMarcada ){
			alert('"Situação" é obrigatória');
			return false; 
		} 
		
		if( undid.value == 322 || undid.value == 22 ){
			
			if( exeqtd.value != ( tabela.rows.length - 1 ) ){
				
				alert( "O campo informações deve conter o mesmo número de linhas do valor registrado no campo quantidade!" );
				return false;
			}
			/*
			if( Number( exeqtd.value ) < 1  ){
				
				alert( 'O campo "Quantidade" deve ser maior que 0.' );
				return false;
			}
			*/
			
			var elem = document.getElementsByTagName( 'input' ); 
			var arr = new Array();  
		    for(i = 0,iarr = 0; i < elem.length; i++) {  
				att = elem[i].getAttribute("name");
				if(att){
					if( att.substr( "eesentqtd", 9 ) == 'eesentqtd') {  
						arr[iarr] = elem[i];  
						iarr++;  
					}  
				}  
			}
			
			for( i=0; i<arr.length; i++ ){
				if( !arr[i].value ){
					alert( 'A quantidade de todas as escolas devem ser preenchidas.' );
					arr[i].focus();
					return false;
				}
			}  
		}
		else{ 
			//if( undid.value == 174 || undid.value == 157 || undid.value == 17 || undid.value == 314 || undid.value == 11 ){ 
			if( in_array( undid.value, arUnidades ) ){
				
				if( tabela ){
					if( exeqtd.value != ( tabela.rows.length - 1 ) ){
								 
						alert( "O campo informações deve conter o mesmo número de linhas do valor registrado no campo quantidade!" );
						return false;
					}
				}
				
				
				if(  Number( exeqtd.value > 0 ) ){ 
				
					if( ( undid.value != <?=CTE_UNIDADE_MEDIDA_REUNIOES?> ) &&( undid.value != <?=CTE_UNIDADE_MEDIDA_DOCUMENTOS?> ) && ( undid.value != <?=CTE_UNIDADE_MEDIDA_KIT_MATERIAL?> ) && ( undid.value != <?=CTE_UNIDADE_MEDIDA_DIAGNOSTICO?> )   ) {
					
						for( i=0; i<exeqtd.value; i++ ){
						 
							var cpf = document.getElementsByName( "eescpf_" + i );
								
							if( ( undid.value  == <?=CTE_UNIDADE_MEDIDA_PROFESSOR_CURSISTA_MUN?>) || (undid.value == <?=CTE_UNIDADE_MEDIDA_PROFESSOR_MULTIPLICADOR_MUN?> ) ){
/*
								var escola = document.getElementsByName( "eesentescola_" + i );

								if( cpf[0].value == '' || escola[0].value == '' ){
								
									alert( "Os campos CPF e Escola são de preenchimento obrigatório!" );
									return false;
								}
*/								
							}else{
							
								if( undid.value != <?=CTE_UNIDADE_MEDIDA_PROFESSOR_MUN?> ){
								/*
									if( cpf[0].value == ''  ){
										alert( "O campo CPF é obrigatório!" );
										return false;
									}
								*/
								}
							}
						}
					}
					
					var elem = document.getElementsByTagName( 'select' ); 
					var arr = new Array();  
				    for(i = 0,iarr = 0; i < elem.length; i++) {  
						att = elem[i].getAttribute("name");
						if(att){
							if( att.substr( "scuid", 5 ) == 'scuid') {  
								arr[iarr] = elem[i];  
								iarr++;  
							}  
						}  
					}
					
					for( i=0; i<arr.length; i++ ){
						if( !arr[i].value ){
							alert( 'A Situação do cursista é de preenchimento obrigatório.' );
							arr[i].focus();
							return false;
						}
					}  
								
				}else{
				/*
					alert('O campo "Quantidade" deve ser maior que 0.');
					return false;
				*/
				}
			}
		}
	
	}else 
		
		var undid 	 = document.getElementById( "undid" );		
	 	var exeqtd 	 = document.formulario.elements['exeqtd'];
		var tabela 	 = document.getElementById( "tabelaEscolas" );
		if( (nrEsdid == '<?php echo EST_CONCLUIDO; ?>' ) &&  (undid.value == <?=CTE_UNIDADE_MEDIDA_PROFESSOR_MULTIPLICADOR_MUN?> )  ){  
		for( i=0; i<exeqtd.value; i++ ){ 
			var cpf = document.getElementsByName( "eescpf_" + i );
			var escola = document.getElementsByName( "eesentescola_" + i );	
			if( undid.value  == <?=CTE_UNIDADE_MEDIDA_PROFESSOR_CURSISTA_MUN?>){
				if( cpf[0].value == '' || escola[0].value == '' ){ 
					alert( "Os campos CPF e Escola são de preenchimento obrigatório!" );
					return false;
				}
			}else{
				if( undid.value != <?=CTE_UNIDADE_MEDIDA_PROFESSOR_MUN?> ){
					/*
					if( cpf[0].value == ''  ){
						alert( "O campo CPF é obrigatório!" );
						return false;
					}
					*/
				}
			}
		} 
	}
	else{
		var momidoutros = document.getElementById('momidoutros');
	
		if( momidoutros ){
		
			var arMomid  = document.getElementsByName( 'momid[]' );
			var empty = Number( 0 );
			for( var k=0; k<arMomid.length; k++ ){  
				if( arMomid[k].checked == true ){
					empty++;
				}
			}    
			if( empty > 0 ){
				for( i=0; i<arMomid.length; i++ ){  
					if( arMomid[i].value == momidoutros.value && arMomid[i].checked ){
						if( document.getElementById( 'momdsc_' + momidoutros.value ).value  == '' ){
							alert('O preenchimento da Observação do motivo "Outros" é obrigatório.');
							document.getElementById( 'momdsc_' + momidoutros.value ).focus();
							return false;
						}
					}
				}
			}else{ 
				if( nrEsdid == '<?php echo EST_NAOINICIADA; ?>' ) {
					alert('É obrigatório a marcação e preenchimento de no mínimo 1 Motivo.');
					return false;
				}
			}
		} 
	}	
	
	var qtdPessoas = document.formulario.exeqtd.value;	
	numLinha = document.getElementsByName('arees[]');	
	totalNumLinha = numLinha.length;
	
	if(totalNumLinha > 0)
	{
		
		for(i=0;i<qtdPessoas;i++)
		{		
			vlrCampo		= numLinha[i].value;
			inputNome 		= document.getElementById( "eesnome_"+vlrCampo );
			inputCPF 		= document.getElementById( "eescpf_"+vlrCampo );
			inputEscola 	= document.getElementById( "eesentescola_"+vlrCampo );
			inputSituacao 	= document.getElementById( "scuid_"+vlrCampo );
			inputEstado 	= document.getElementById( "estid" );
			inputAcumulado 	= document.getElementById( "exedtexecucao_mes" );						
			
			if(inputCPF)
			{
				
				if(inputEstado.value == 2 || inputEstado.value == 5)
				{					
					//Não permite CPF repetido
					for(x=0;x<i;x++)
					{						
						vlrCampoCpf1		= numLinha[x].value;
						vlrCampoCpf2		= numLinha[i].value;
						inputCPFatual 		= document.getElementById( "eescpf_"+vlrCampoCpf1 );
						inputCPFanterior 	= document.getElementById( "eescpf_"+vlrCampoCpf2 );										
						CPFatual 			= retiraPontos(inputCPFatual.value);
						CPFanterior 		= retiraPontos(inputCPFanterior.value);																								
						
						if (CPFatual == CPFanterior)
						{
							alert("O campo CPF não pode conter valores repetidos!");
							//inputCPFatual.focus();
							inputCPFanterior.focus();
							return false;
						}										
					}
					
					//CPF obrigatório
					if(inputCPF != null)
					{				
						if(inputCPF.value == '')
						{
							alert("O campo CPF não pode estar em branco!");
							inputCPF.focus();
							return false;
						}				
					}
					
					
					if(inputNome != null)
					{
						if(inputNome.value == '')
						{
							alert("O campo NOME não pode estar em branco!");
							inputNome.focus();
							return false;
						}
					}
					
					if(inputEscola != null)
					{
						if(inputEscola.value == '' || inputEscola.value == 'Selecione')
						{
							alert("O campo ESCOLA não pode estar em branco!");
							inputEscola.focus();
							return false;
						}
					}
					/*
					if(inputSituacao != null)
					{
						if(inputSituacao.value == '' || inputSituacao.value == 'Selecione')
						{
							alert("O campo SITUAÇÃO não pode estar em branco!");
							inputSituacao.focus();
							return false;
						}
					}
					*/				
					if(inputAcumulado != null)
					{
						if(inputAcumulado.value == '' || inputAcumulado.value == 'Selecione...')
						{
							alert("O campo ACUMULADO não pode estar em branco!");
							inputAcumulado.focus();
							return false;
						}
					}
				}
			}//condição cpf
			
		}
		
	}
	
	document.getElementById('btnsalvar').disabled=true;
	document.formulario.mntidT.value = navegarSub;
	document.getElementById('formulario').submit();
}
function retiraPontos(string) {
        var i = 0;
        var final = '';
        while (i < string.length) {
                if (string.charAt(i) == '.' || string.charAt(i) == '-') {
                        final += string.substr(0, i);
                        string = string.substr(i+1, string.length - (i+1));
                        i = 0;
                }
                else {
                        i++;
                }
        }
        return final + string;
}
function validarrestricao() {
	if(document.formulario.elements['resdsc'].value == '') {
		alert('"Restrição" é obrigatório');
		return false;
	}
	if(document.getElementById('trscod').value == '') {
		alert('"Tipo de restrição" é obrigatório');
		return false;
	}
				
	document.getElementById('btnsalvar').disabled=true;
	document.getElementById('formulario').submit();
}

function enviar_email( cpf ){
	var nome_janela = 'janela_enviar_emai_' + cpf;
	window.open(
		'/geral/envia_email.php?cpf=' + cpf,
		nome_janela,
		'width=650,height=557,scrollbars=yes,scrolling=yes,resizebled=yes'
	);
}

function popupSelecionarEscolas( muncod ){

	exeqtd = document.getElementById('exeqtd').value;	
	
	return windowOpen( 	'/cte/cte.php?modulo=principal/relacionarEntidadeExecucao&acao=A&muncod=' + muncod + '&exeqtd=' + exeqtd,
						'popupRelacionarEntidadeExecucao',
						'height=600, width=600, status=yes, toolbar=no, menubar=no, scrollbars=yes, location=no, resizable=yes' );
	return false;
}

function removerLinha( idLinha ){

	if( confirm( "Deseja realmente excluir o registro?" ) ){
	
		var tabela 	= document.getElementById( "tabelaEscolas" );
		var linha 	= document.getElementById( idLinha );
				
		tabela.deleteRow( linha.rowIndex );
		//tabela.deleteRow( idLinha );
	}

}

function procurarNome( cpf, qtd ){
	var data = '';
	var comp = new dCPF();
	var arNome = document.getElementsByName( 'eesnome_' + qtd );
	
	comp.buscarDados( cpf );
	if (comp.dados.no_pessoa_rf != ''){
		arNome[0].value = comp.dados.no_pessoa_rf;
		arNome[0].readOnly = true;
	}
}

function eventosIE( obj, tipo ){
	
	switch( tipo ){
		case('1'): MouseBlur(obj); validarEescpf( obj ); break;
		case('2'): MouseOut(obj); break;
		case('3'): MouseClick(obj); obj.select(); break;
		case('4'): MouseOver(obj); break;
		case('5'): obj.value=mascaraglobal('###.###.###-##',obj.value); break;
	}
	
}

function eventosIECPF( obj, qtd ){
	procurarNome( obj.value, qtd );
}

function carregarQuantidade(unidadeSemPolo){
	var exeqtd 	= document.formulario.elements['exeqtd'];
	var tabela 	= document.getElementById( "tabelaEscolas" );
	
	var qtdLinha =  tabela.rows.length - 1;
	
	if( exeqtd.value > qtdLinha ){
		
        return new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: '&req=montarCombos&qtd=' + qtdLinha +'&unidadeSemPolo='+unidadeSemPolo,
                                       onComplete: function( res )
                                       {
											//var qtd = qtdLinha;
											//var qtd = tabela.rows.length;
											var numLinha = document.getElementsByName('arees[]');
											var ultNumLinha = numLinha.length;
											
											if(ultNumLinha>0){
												var numAtual = ultNumLinha-1; 											
												var vlrLinha = numLinha[numAtual].value;
												var qtd = parseInt(vlrLinha)+1; 
											} else {
												var qtd = tabela.rows.length;
											}														
										
											var nrLinha = tabela.rows.length;
											
											linha = tabela.insertRow( tabela.rows.length );

											linha.setAttribute( "id", "linha_" + qtd );
											linha.setAttribute( "style", "background: #f5f5f5;" );
											linha.setAttribute( "align", "center" );
											

											var img = document.createElement( "img" );
											img.setAttribute( "src", "/imagens/excluir.gif" );
											img.setAttribute( "title", "Excluir" );
											img.setAttribute( "alt", "Excluir" );

											// OnBlur
											if(window.addEventListener){ // Mozilla, Netscape, Firefox
												img.setAttribute( "onclick", "removerLinha( 'linha_" + qtd + "' )" );
												//img.setAttribute( "onclick", "removerLinha( this.parentNode.parentNode.rowIndex )" );
											}
											else{ // IE
												img.attachEvent( "onclick", function() { removerLinha( "'linha_" + qtd + "'"  ) } );
												//img.attachEvent( "onclick", function() { removerLinha( this.parentNode.parentNode.rowIndex ) } );
											}
															
											var cpf = document.createElement( "input" );
											cpf.setAttribute( "type", "text" );
											cpf.setAttribute( "name", "eescpf_" + qtd );
											cpf.setAttribute( "id", "eescpf_" + qtd );
											cpf.setAttribute( "style", "width: 21ex;" );
											cpf.setAttribute( "size", "19" );
											cpf.className = "normal eescpf";
											
											// OnBlur
											if(window.addEventListener){ // Mozilla, Netscape, Firefox
												cpf.setAttribute( "onblur", "MouseBlur(this); validarEescpf( this );" );
											}
											else{ // IE
												cpf.attachEvent( "onblur", function() { eventosIE( document.getElementById( 'eescpf_' + qtd ), "1" ); } );
												
											}
											
											// OnMouseOut
											if(window.addEventListener){ // Mozilla, Netscape, Firefox
												cpf.setAttribute( "onmouseout", "MouseOut(this);" );
											}
											else{ // IE
												cpf.attachEvent( "onmouseout", function() { eventosIE( document.getElementById( 'eescpf_' + qtd ), "2" ); } );
											}
											
											// OnFocus
											if(window.addEventListener){ // Mozilla, Netscape, Firefox
												cpf.setAttribute( "onfocus", "MouseClick(this); this.select();" );
											}
											else{ // IE

												cpf.attachEvent( "onfocus", function() { eventosIE( document.getElementById( 'eescpf_' + qtd ), "3" ); } );
											}
											
											// OnMouseOver
											if(window.addEventListener){ // Mozilla, Netscape, Firefox
												cpf.setAttribute( "onmouseover", "MouseOver(this);" );
											}
											else{ // IE
												cpf.attachEvent( "onmouseover", function() { eventosIE( document.getElementById( 'eescpf_' + qtd ), "4" ); } );
											}
											
											// OnKeyUp
											if(window.addEventListener){ // Mozilla, Netscape, Firefox
												cpf.setAttribute( "onkeyup", "this.value=mascaraglobal('###.###.###-##',this.value);" );
											}
											else{ // IE
												cpf.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'eescpf_' + qtd ), "5" ); } );
											}
											
											// OnChange
											if(window.addEventListener){ // Mozilla, Netscape, Firefox
												cpf.setAttribute( "onchange", "procurarNome( this.value, "+ qtd +" )" );
											}
											else{ // IE
												cpf.attachEvent( "onblur", function() { procurarNome( cpf.value, qtd ); } );												
											}
											
											// OnKeyUp
											if(window.addEventListener){ // Mozilla, Netscape, Firefox
												cpf.setAttribute( "onkeyup", "this.value=mascaraglobal('###.###.###-##',this.value);" );
											}
											else{ // IE
												cpf.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'eescpf_' + qtd ), "5" ); } );
											}

											var nome = document.createElement( "input" );
											nome.setAttribute( "type", "text" );
											nome.setAttribute( "readonly", "readonly" );
											nome.setAttribute( "name", "eesnome_" + qtd );
											nome.setAttribute( "id", "eesnome_" + qtd );
											nome.setAttribute( "style", "width: 33ex;" );
											nome.setAttribute( "size", "30" );
											nome.className = "disabled";
											
											var eesentescola = document.createElement( "input" );
											eesentescola.setAttribute( "type", "hidden" );
											eesentescola.setAttribute( "name", "eesentescola_" + qtd );
											eesentescola.setAttribute( "value", qtd );
											
											var arees = document.createElement( "input" );
											arees.setAttribute( "type", "hidden" );
											arees.setAttribute( "name", "arees[]" );
											arees.setAttribute( "value", qtd );
                                       		
											celula = linha.insertCell( 0 );
											celula.appendChild( img );
											celula.appendChild( arees );
											
											var imgObrigatorio = document.createElement( "img" );
											imgObrigatorio.setAttribute( "src", "../imagens/obrig.gif" );
											imgObrigatorio.setAttribute( "title", "Indica campo obrigatório." );												
											
											celula = linha.insertCell( 1 );
											celula.appendChild( cpf );
											celula.appendChild( imgObrigatorio );
											
											celula = linha.insertCell( 2 );
											celula.appendChild( nome );
											
											eval( res.responseText );

											if(unidadeSemPolo == 0 || unidadeSemPolo == 1){
												comboPolo.setAttribute( 'name', 'polid_' + qtd );
												comboPolo.setAttribute( 'id', 'polid_' + qtd );												
												
												comboEscola.setAttribute( 'name', 'eesentescola_' + qtd );
												comboEscola.setAttribute( 'id', 'eesentescola_' + qtd );
												comboEscola.setAttribute( 'style', 'width: 240px' );												
												
												comboSituacaoCursista.setAttribute( 'name', 'scuid_' + qtd );
												comboSituacaoCursista.setAttribute( 'id', 'scuid_' + qtd );										
											}
											
										}
                                   });										
	}

}

function validarEescpf( obj ){ 
	if( obj.value ){
		if( !validar_cpf( obj.value  ) ){
			alert( "CPF inválido!\nFavor informar um cpf válido!" );
			obj.value = "";	
		}
	}
}
-->
</script>
<style type="text/css" >
	.camposExecucao{
		margin-right: 20px;
	}
</style>
<?php
switch($_REQUEST['_page']) {
	
/***************** 
 * ABA DE EXECUÇÃO 
 *****************/
	case 'execucao':
		if(!$_REQUEST['perano']) {
			$_REQUEST['perano'] = date("Y");
		}
		$sql = "-- CRONOGRAMA GLOBAL
				SELECT ume.unddsc as unidademedida, 
						ume.undid,
						frm.frmdsc as formaexecucao, 
						si.sbadsc as descricaosubacao, 
						spt.sptano as ano, 
						spt.sptunt as quantidade, 
						spt.sptinicio as mesinicio, 
						spt.sptfim as mesfim,
						spt.sptanoterminocurso as anotermino,
						sts.ssudescricao, 
						spt.ssuid
        		FROM cte.pontuacao p
	        		INNER JOIN cte.acaoindicador ai ON ai.ptoid = p.ptoid
	        		INNER JOIN cte.subacaoindicador si ON si.aciid = ai.aciid
	        		LEFT  JOIN cte.formaexecucao frm ON si.frmid = frm.frmid
	        		INNER JOIN cte.unidademedida ume ON si.undid = ume.undid
	        		INNER JOIN cte.subacaoparecertecnico spt ON si.sbaid = spt.sbaid
	        		LEFT JOIN cte.statussubacao sts ON spt.ssuid = sts.ssuid
        		WHERE p.ptostatus = 'A' 
        				AND si.sbaid = '".$subac['sbaid']."' 
        				AND coalesce(spt.sptano, 0) <> 0 
        				AND coalesce(spt.sptano, 0) = ".$_REQUEST['perano']."
        				AND si.sbaporescola = false AND coalesce(spt.ssuid, 0) = 3
					UNION ALL
					-- CRONOGRAMA POR ESCOLA
					SELECT ume.unddsc as unidademedida, 
							ume.undid,
							frm.frmdsc as formaexecucao, 
							si.sbadsc as descricaosubacao, 
							spt.sptano as ano, 
							count(1) as quantidade, 
							spt.sptinicio as mesinicio, 
							spt.sptfim as mesfim,
							spt.sptanoterminocurso as anotermino,
							sts.ssudescricao,
							spt.ssuid
	        		FROM cte.pontuacao p
		        		INNER JOIN cte.acaoindicador ai ON ai.ptoid = p.ptoid
		        		INNER JOIN cte.subacaoindicador si ON si.aciid = ai.aciid
		        		LEFT JOIN cte.formaexecucao frm ON si.frmid = frm.frmid
		        		INNER JOIN cte.unidademedida ume ON si.undid = ume.undid
		        		INNER JOIN cte.subacaoparecertecnico spt ON si.sbaid = spt.sbaid
		        		LEFT JOIN cte.qtdfisicoano qfa ON si.sbaid = qfa.sbaid
		        		LEFT JOIN cte.statussubacao sts ON spt.ssuid = sts.ssuid
	        		WHERE p.ptostatus = 'A' 
	        			AND si.sbaid = '".$subac['sbaid']."' 
	        			-- AND coalesce(qfa.qfaano, 0) <> 0
	        			--AND coalesce(qfa.qfaano, 0) = ".$_REQUEST['perano']."
          				AND coalesce(spt.sptano, 0) = ".$_REQUEST['perano']." 
	        			--AND coalesce(spt.ssuid, 0) = 3
					GROUP BY ume.unddsc, ume.undid, frm.frmdsc, si.sbadsc, spt.sptano, spt.sptinicio, spt.sptfim, spt.sptanoterminocurso, sts.ssudescricao, spt.ssuid";
 
		$subacaoindicador = $db->carregar($sql);
		if($subacaoindicador[0]) {
			$subacaoindicador = $subacaoindicador[0];
		} else {
			unset($subacaoindicador);
		}
		$abas_ano = array(0 => array("id"=>1,"descricao"=>"2008","link"=>"?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid']."&perano=2008"),
						  1 => array("id"=>2,"descricao"=>"2009","link"=>"?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid']."&perano=2009"),
						  2 => array("id"=>3,"descricao"=>"2010","link"=>"?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid']."&perano=2010"),
						  3 => array("id"=>4,"descricao"=>"2011","link"=>"?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid']."&perano=2011")
						  );
		echo montarAbasArray($abas_ano, "?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid']."&perano=".$_REQUEST['perano']);
		
//ver($subac['sbaid']."_sbaid", $_SESSION['cte_var']['mntid']."_mntid", $_REQUEST['perid']."_perid", $subacaoindicador['ano']."sptano");

$peridAnterior = "";
$anoAnterior = $_REQUEST['perano']-1;

switch ($anoAnterior) {
	
	case 2008: $peridAnterior = 2;
	break;
	case 2009: $peridAnterior = 4;
	break;
	case 2010: $peridAnterior = 6;
	break;
	case 2011: $peridAnterior = 8;
	break;
	
}

if($peridAnterior != ""){
	
	$sql = "SELECT sptano FROM cte.subacaoparecertecnico WHERE sbaid = '".$subac['sbaid']."' AND sptano = '".$_REQUEST['perano']."'";
	$parecerTecnicoAtual = $db->pegaLinha($sql);
	
	$sql = "SELECT exeid FROM cte.execucaosubacao WHERE mntid = '".$_SESSION['cte_var']['mntid']."' AND perid = '".$peridAnterior."' AND estid = '2' AND exeatual = 't'";
	$situacaoAnterior = $db->pegaLinha($sql);
}

if(!$parecerTecnicoAtual && $situacaoAnterior){
	
	$sql = "SELECT 
				und.unddsc as unidademedida, 
				frm.frmdsc as formaexecucao, 
				spt.sptinicio as mesinicio, 
				spt.sptfim as mesfim,
				spt.sptano as ano,
				spt.sptanoterminocurso as anotermino,
				spt.sptunt as quantidade,
				sts.ssudescricao,
				spt.ssuid  
			FROM cte.subacaoindicador sba
			INNER JOIN cte.unidademedida und 			ON und.undid = sba.undid
			INNER JOIN cte.formaexecucao frm 			ON frm.frmid = sba.frmid
			INNER JOIN cte.subacaoparecertecnico spt 	ON spt.sbaid = sba.sbaid
			LEFT JOIN cte.statussubacao sts 			ON spt.ssuid = sts.ssuid
			WHERE sba.sbaid = '".$subac['sbaid']."' AND spt.sptano = '".$anoAnterior."'";
	$infoPeriAnterior = $db->pegaLinha($sql);
	
	$subacaoindicador['unidademedida'] 	= $infoPeriAnterior['unidademedida'];
	$subacaoindicador['formaexecucao'] 	= $infoPeriAnterior['formaexecucao'];
	$subacaoindicador['mesinicio'] 		= $infoPeriAnterior['mesinicio'];
	$subacaoindicador['mesfim'] 		= $infoPeriAnterior['mesfim'];
	$subacaoindicador['ano'] 			= $infoPeriAnterior['ano'];
	$subacaoindicador['anotermino'] 	= $infoPeriAnterior['anotermino'];	
	$subacaoindicador['quantidade']		= $infoPeriAnterior['quantidade'];
	$subacaoindicador['ssudescricao']	= $infoPeriAnterior['ssudescricao'];
}

?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td class="SubTituloEsquerda" colspan="4">Dados da subação</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="120">Unidade de medida :</td>
		<?
		if( $_REQUEST['perano'] > date("Y") ){
			$color = "style=\"color: gray;\"";
		}else{
			$color = "style=\"color: black;\"";
		}
		?>
		<td <?=$color;?> ><? echo (($subacaoindicador['unidademedida'])?$subacaoindicador['unidademedida']:"Não preenchido"); ?></td>
		<td class="SubTituloDireita" width="120">Forma de execução :</td>
		<td colspan="3" <?=$color;?>><? echo (($subacaoindicador['formaexecucao'])?$subacaoindicador['formaexecucao']:"Não preenchido"); ?></td>
	</tr>
	<tr>
		<?php
		if ($subacaoindicador['anotermino'] != 0 && $subacaoindicador['anotermino'] != '')
		{
			$anoTermino = $subacaoindicador['anotermino'];
		} else {
			$anoTermino = $subacaoindicador['ano'];
		}
		?>
		<td class="SubTituloDireita">Início :</td>
		<td <?=$color;?> >
			<? echo (($subacaoindicador['mesinicio']&&$subacaoindicador['ano'])?$subacaoindicador['mesinicio']."/".$subacaoindicador['ano']:"Não preenchido"); ?>
		</td>
		<td class="SubTituloDireita">Fim :</td>
		<td <?=$color;?>>
			<? echo (($subacaoindicador['mesfim'] && $anoTermino)?$subacaoindicador['mesfim']."/".$anoTermino:"Não preenchido"); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Status :</td>
		<td <?=$color;?> >
			<?php echo (($subacaoindicador['ssudescricao'])?$subacaoindicador['ssudescricao']:"Não preenchido"); ?>		
		</td>
		<td class="SubTituloDireita"> </td>
		<td <?=$color;?>>
			
		</td>
	</tr>
	<?php
	$sql = "SELECT * FROM cte.periodoreferencia WHERE SUBSTR(perdtinicioref,1,4)='".$_REQUEST['perano']."' OR SUBSTR(perdtterminoref,1,4)='".$_REQUEST['perano']."' ORDER BY perid";
	$periodosreferencias = $db->carregar($sql);
	?>
	<tr>
		<td class="SubTituloEsquerda" colspan="4">Execução da subação</td>
	</tr>
	<tr>
		<td colspan="4">
			<? if($periodosreferencias[0]) { ?>
			<table cellspacing="2" cellpadding="2" align="center" width="100%">
				<tr>
					<?php
					foreach($periodosreferencias as $per) {
						if($per['perid']==$_REQUEST['perid']) {
							$perdscselecionado = $per['perdsc'];
						}
						
						echo "<td class=\"SubTituloDireita\">".$per['perdsc']."</td>";
					}
					
					
					?>
					<td class="SubTituloDireita">Previsto</td>
					<td class="SubTituloDireita">Executado</td>
					<td class="SubTituloDireita">% Exec.</td>
				</tr>
				<tr bgcolor="#ffffff">
					<?	
					$qtdtotal = 0;
					foreach($periodosreferencias as $per) { 
						
						$sql = "SELECT exe.exeqtd, cor.corimgav FROM cte.execucaosubacao exe
								LEFT JOIN cte.cor cor ON cor.corcod = exe.corcod 
								WHERE perid='".$per['perid']."' AND mntid='".$_SESSION['cte_var']['mntid']."' AND exeatual=TRUE";						
						$execsub = $db->carregar($sql);
						
						$sql = "SELECT perid 
									FROM cte.execucaosubacao 
									WHERE exeatual=TRUE 
									AND mntid='".$_SESSION['cte_var']['mntid']."'									
									AND estid IN (5)
									ORDER BY perid ASC";
						$perConcluido = $db->pegaUm($sql, 0);
						
						$sql = "SELECT 	
									CASE WHEN sptanoterminocurso is not null AND sptfim < 10
										THEN sptanoterminocurso || '0' || sptfim
									WHEN sptanoterminocurso is not null AND sptfim >= 10
										THEN sptanoterminocurso || '' || sptfim
									END as anoTermino
								FROM cte.subacaoparecertecnico 
								WHERE sptano = '".$_REQUEST['perano']."'
								AND sbaid = '".$subac['sbaid']."'
								AND sptanoterminocurso is not null
								AND sptanoterminocurso != '0'";
						$perAnoTermino = $db->pegaUm($sql, 0);
						
						if(!$perAnoTermino){
							$peranoAnterior = $_REQUEST['perano']-1;
							$sql = "SELECT 	
									CASE WHEN sptanoterminocurso is not null AND sptfim < 10
										THEN sptanoterminocurso || '0' || sptfim
									WHEN sptanoterminocurso is not null AND sptfim >= 10
										THEN sptanoterminocurso || '' || sptfim
									END as anoTermino
								FROM cte.subacaoparecertecnico 
								WHERE sptano = '".$peranoAnterior."'
								AND sbaid = '".$subac['sbaid']."'
								AND sptanoterminocurso is not null
								AND sptanoterminocurso != '0'";
							$perAnoTermino = $db->pegaUm($sql, 0);	
						}						 
						
						$sql = "SELECT perdtinicioref 
								FROM cte.periodoreferencia 
								WHERE perid = '".$per['perid']."'";
						$perDataRefFim = $db->pegaUm($sql, 0);							
						
//						ver($execsub, $per['perid']." > ".$perConcluido." && ".$perDataRefFim." > ".$perAnoTermino);
						if($execsub[0]) {
							$execsub = $execsub[0];
							$qtdtotal += $execsub['exeqtd'];
							$corIcone = ( trim( $execsub['corimgav'] ) == 'av_branco.gif' ) ? 'av_preto.gif' : $execsub['corimgav'];
							echo "<form id='ref_".$per['perid']."' method='post' action='?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid']."&perano=".$_REQUEST['perano']."'>";
							echo "<input type='hidden' name='perid' value='".$per['perid']."'>";													
							//echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"document.getElementById('ref_".$per['perid']."').submit();\">".$execsub['exeqtd']." <img align=\"absmiddle\" src=\"../imagens/". $corIcone ."\" width=\"25\" height=\"10\"></td>";							
							
							/*******************************************
							 * REGRA PARA TRAVAR PERÍODOS POSTERIORES AO
							 * PERÍODO DE CONCLUSÃO
							 */							
							if(!$perConcluido && !$perAnoTermino){
								echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"document.getElementById('ref_".$per['perid']."').submit();\">".$execsub['exeqtd']." <img align=\"absmiddle\" src=\"../imagens/". $corIcone ."\" width=\"25\" height=\"10\"></td>";
							} elseif($perConcluido && $perAnoTermino) {		
//								if($per['perid'] > $perConcluido && $perDataRefFim > $perAnoTermino){
								if($per['perid'] > $perConcluido){
									echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"javascript:alert('Monitoramento concluído.');\">".$execsub['exeqtd']." <img align=\"absmiddle\" src=\"../imagens/". $corIcone ."\" width=\"25\" height=\"10\"></td>";
								} else {
									echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"document.getElementById('ref_".$per['perid']."').submit();\">".$execsub['exeqtd']." <img align=\"absmiddle\" src=\"../imagens/". $corIcone ."\" width=\"25\" height=\"10\"></td>";								
								}	
							} elseif($perConcluido && !$perAnoTermino){
								if($per['perid'] > $perConcluido){
									echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"javascript:alert('Monitoramento concluído.');\">".$execsub['exeqtd']." <img align=\"absmiddle\" src=\"../imagens/". $corIcone ."\" width=\"25\" height=\"10\"></td>";
								} else {
									echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"document.getElementById('ref_".$per['perid']."').submit();\">".$execsub['exeqtd']." <img align=\"absmiddle\" src=\"../imagens/". $corIcone ."\" width=\"25\" height=\"10\"></td>";								
								}
							} else {
								//echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"document.getElementById('ref_".$per['perid']."').submit();\"> - <img align=\"absmiddle\" src=\"../imagens/av_preto.gif\" width=\"25\" height=\"10\"></td>";
								echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"document.getElementById('ref_".$per['perid']."').submit();\">".$execsub['exeqtd']." <img align=\"absmiddle\" src=\"../imagens/". $corIcone ."\" width=\"25\" height=\"10\"></td>";
							}						
							
							echo "</form>";	
						} else {
							echo "<form id='ref_".$per['perid']."' method='post' action='?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid=".$_SESSION['cte_var']['mntid']."&perano=".$_REQUEST['perano']."'>";
							echo "<input type='hidden' name='perid' value='".$per['perid']."'>";
							//echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"document.getElementById('ref_".$per['perid']."').submit();\"> - <img align=\"absmiddle\" src=\"../imagens/av_preto.gif\" width=\"25\" height=\"10\"></td>";
							
							/*******************************************
							 * REGRA PARA TRAVAR PERÍODOS POSTERIORES AO
							 * PERÍODO DE CONCLUSÃO
							 */
							if(!$perConcluido && !$perAnoTermino){
								echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"document.getElementById('ref_".$per['perid']."').submit();\"> - <img align=\"absmiddle\" src=\"../imagens/av_preto.gif\" width=\"25\" height=\"10\"></td>";
							} elseif($perConcluido && $perAnoTermino) {
//								if($per['perid'] > $perConcluido && $perDataRefFim > $perAnoTermino){
								if($per['perid'] > $perConcluido){
									echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"javascript:alert('Monitoramento concluído.');\"> - <img align=\"absmiddle\" src=\"../imagens/av_preto.gif\" width=\"25\" height=\"10\"></td>";
								} else {
									echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"document.getElementById('ref_".$per['perid']."').submit();\"> - <img align=\"absmiddle\" src=\"../imagens/av_preto.gif\" width=\"25\" height=\"10\"></td>";								
								}	
							} elseif($perConcluido && !$perAnoTermino){
								if($per['perid'] > $perConcluido){									
									echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"javascript:alert('Monitoramento concluído.');\"> - <img align=\"absmiddle\" src=\"../imagens/av_preto.gif\" width=\"25\" height=\"10\"></td>";
								} else {
									echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"document.getElementById('ref_".$per['perid']."').submit();\"> - <img align=\"absmiddle\" src=\"../imagens/av_preto.gif\" width=\"25\" height=\"10\"></td>";								
								}
							} else {
								echo "<td align=\"right\" style=\"cursor:pointer;font-size: 9px;\" onclick=\"document.getElementById('ref_".$per['perid']."').submit();\">".$execsub['exeqtd']." - <img align=\"absmiddle\" src=\"../imagens/av_preto.gif\" width=\"25\" height=\"10\"></td>";
							}						
								
							echo "</form>";
						}
				
				 	}
				 	
				 	//aqui
					if(  ( $subacaoindicador['ano'] == date("Y") ) && ( $subacaoindicador['mesinicio'] > 6 ) ){
						$permissaogravar  = 'N';
						//$subQtd = "Não preenchido";
					}else{
						$permissaogravar  = 'S';
						//$subQtd = $subacaoindicador['quantidade'];
					}					
					
					if ($subacaoindicador['quantidade'] > 0 || $subacaoindicador['quantidade'] != null)
					{
						$subQtd = $subacaoindicador['quantidade'];
					} else {
						$subQtd = "Não preenchido";
					}

					if(!$parecerTecnicoAtual && $situacaoAnterior){
						$sql = "SELECT sum(exeqtd) FROM cte.execucaosubacao WHERE mntid = '".$_SESSION['cte_var']['mntid']."' AND exeatual = 't'";
						$subQtdNova = $db->pegaLinha($sql);
						$qtdtotal = $subQtdNova['sum'];
					}
				 	
				 	?>
				 	<td align="right"><? echo $qtdPrevista['previsto'] = $qtdPrevista['previsto'] ? $qtdPrevista['previsto'] : 0; //$subQtd ?></td>
				 	<td align="right"><? echo $qtdExecutada['executado'] = $qtdExecutada['executado'] ? $qtdExecutada['executado'] : 0; //(($qtdtotal)?$qtdtotal:"0"); ?></td>
				 	<td align="right"><? echo $qtdPrevista['previsto'] > 0 ? round(($qtdExecutada['executado']/$qtdPrevista['previsto'])*100, 2) : 0; //(($subacaoindicador['quantidade'] > 0)?round($qtdtotal/$subacaoindicador['quantidade']*100)."%":"0%"); ?></td>
				</tr>
			</table>
			<?php
			} else {
				echo "<strong>Não existem períodos de referências cadastrados</strong>";
			}
			?>
		</td>
	</tr>
</table>
<?
if($_REQUEST['perid']){	
	
	$sql = "SELECT * FROM cte.execucaosubacao 
			WHERE mntid = '".$_SESSION['cte_var']['mntid']."'
			AND perid = '".$_REQUEST['perid']."'
			AND exeatual = true";
			$execucaoSubacao = $db->carregar($sql);

	if ($_REQUEST['removeSubacao']){		
		
		$sql = "UPDATE cte.execucaosubacao SET exeatual = false WHERE exeid = '".$execucaoSubacao[0]['exeid']."'";		
		if($db->executar($sql)){
			
			alert("Monitoramento removido com sucesso!");
			echo '<script>';
			echo 'document.location.href = "cte.php?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid='.$_SESSION['cte_var']['mntid'].'&perano='.$_REQUEST['perano'].'&perid='.$_REQUEST['perid'].'";';
			echo '</script>';
		}			
	}
}

if($_REQUEST['perid']) { 
	$sql = "SELECT perid, SUBSTR(perdtinicioref,5,2) as perdtinicioref_mes, 
	SUBSTR(perdtinicioref,1,4) as perdtinicioref_ano, 
	SUBSTR(perdtterminoref,1,4) as perdtterminoref_ano, 
	SUBSTR(perdtterminoref,5,2) as perdtterminoref_mes 
	FROM cte.periodoreferencia 
	WHERE perid='".$_REQUEST['perid']."'   ";
	//AND NOW() >= perdtiniciocad AND NOW() <= perdtterminocad
	$peri = $db->carregar($sql);
	 
	if($peri[0]) {
		foreach($peri as $p) {
			$permissaoperreferencia[$p['perdtinicioref_ano']]['i'] = $p['perdtinicioref_mes'];
			$permissaoperreferencia[$p['perdtterminoref_ano']]['f'] = $p['perdtterminoref_mes'];
		}
	}
	
	$sql = "SELECT exeid, SUBSTR(exedtexecucao,5,2) as exedtexecucao_mes, estid, exeqtd, exeobs, corcod FROM cte.execucaosubacao WHERE exeatual=TRUE AND mntid='".$_SESSION['cte_var']['mntid']."' AND perid = '".$_REQUEST['perid']."'";			
	$execucao = $db->pegaLinha($sql);
	
	$estid = $execucao['estid'];
	$estidAtual = $execucao['estid'];
	switch($estid) {
		case EST_NAOINICIADA:
		case EST_SUSPENSO:
		case EST_CANCELADO:
			$layoutmotivo = true;	
			break;
		default:
			$layoutmotivo = false;
	}
	$exedtexecucao_mes = $execucao['exedtexecucao_mes'];
	$exeqtd = $execucao['exeqtd'];
	$exeid = $execucao['exeid'];
	$exeobs = $execucao['exeobs'];
	$corcod = $execucao['corcod'];
	
	if($permissaoperreferencia[$_REQUEST['perano']] && isset($subacaoindicador) ) { 
		$permissaogravar = 'S';
	} else {
		$permissaogravar = 'N';
	}

	$obEntidadeExecucaoSubacao = new EntidadeExecucaoSubacao();
	
	$stCampos = "eesnome, eescpf, polid, eesentescola, eesentqtd, scuid";
	
	$coEntidadeExecucaoSubacao = array();
	if( $exeid ){
		$arCondicoes = array( "exeid = $exeid" );
		$coEntidadeExecucaoSubacao = $obEntidadeExecucaoSubacao->recuperarTodos( $stCampos, $arCondicoes );
	} else {
		
		$perAnterior = $_REQUEST['perid'] - 1;
		if(!$perAnoTermino){
			$sql = "SELECT exeid, SUBSTR(exedtexecucao,5,2) as exedtexecucao_mes, estid, exeqtd, exeobs, corcod 
			FROM cte.execucaosubacao 
			WHERE exeatual=TRUE 
			AND mntid='".$_SESSION['cte_var']['mntid']."' 
			AND perid = '{$perAnterior}'
			AND estid IN (2, 5)		
			ORDER BY perid DESC";
		} else {
			$sql = "SELECT exeid, SUBSTR(exedtexecucao,5,2) as exedtexecucao_mes, estid, exeqtd, exeobs, corcod 
			FROM cte.execucaosubacao 
			WHERE exeatual=TRUE 
			AND mntid='".$_SESSION['cte_var']['mntid']."' 
			AND exedtexecucao < '{$perAnoTermino}'
			AND estid IN (2, 5)		
			ORDER BY perid DESC";
		}
		
		$execucao = $db->pegaLinha($sql);		
		
		//$exedtexecucao_mes = $execucao['exedtexecucao_mes'];
		$exeqtd = $execucao['exeqtd'];
		$exeid = $execucao['exeid'];
		$estid = $execucao['estid'];
		//$exeobs = $execucao['exeobs'];
		//$corcod = $execucao['corcod'];
		
		if($permissaoperreferencia[$_REQUEST['perano']] && isset($subacaoindicador) ) { 
			$permissaogravar = 'S';
		} else {
			$permissaogravar = 'N';
		}
		if($exeid)
		{
			$arCondicoes = array( "exeid = $exeid" );
			$coEntidadeExecucaoSubacao = $obEntidadeExecucaoSubacao->recuperarTodos( $stCampos, $arCondicoes );			
		}		
		
	}
	$undid = $subacaoindicador["undid"]; 
	
	//montando label
	
	if( $peri[0]['perdtinicioref_mes'] > 6 ){
		$semestre = "2º";

	}elseif(  ($peri[0]['perdtinicioref_mes'] != '' ) &&(  $peri['perdtinicioref_mes'] <= 6)){
		$semestre = "1º";	
	} 
	$soConsulta = false;	 
	$perfis = cte_arrayPerfil();
	if( ( count($perfis == 1)) && ( in_array(CTE_PERFIL_CONSULTA_GERAL, $perfis ) 	|| 
									in_array(CTE_PERFIL_CONSULTA_ESTADUAL, $perfis ) ||
									in_array(CTE_PERFIL_ADMINISTRADOR, $perfis ) || 
									in_array(CTE_PERFIL_CONSULTA_MUNICIPAL, $perfis ) )){
		//não altera
		$soConsulta = true;
	}else{
		//pode alterar
		$soConsulta = false;
	}	
	//fim montando label
	$labelSemestre = $semestre.' Semestre de '.$_REQUEST['perano'];
	if( ( $subacaoindicador['ano'] == date("Y") ) && ( $subacaoindicador['mesinicio'] > 6 ) && $soConsulta){
		$foraDoPrazo = true;
		$permissaogravar  = 'N';
		$subQtd = "Não preenchido";
	}elseif( $subacaoindicador && !$soConsulta){		 
		$permissaogravar  = 'S';
		$foraDoPrazo = false;
		$subQtd = $subacaoindicador['quantidade'];
	}else{		 
		$permissaogravar  = 'N';
		$foraDoPrazo = false;
		$subQtd = "Não preenchido";
	}
	
	/*******************************************************************
	 * LIBERA NOVAMENTE O PREENCHIMENTO PARA 2008 E 1º SEMESTRE DE 2009
	 */	
	if($_REQUEST['perid'] < 5)
	{
		$permissaogravar = "S";
	} else {
		$permissaogravar = "N";
	}
	
	/****************************************
	 * VALIDAÇÃO SEGUNDO DATAS DO CRONOGRAMA 
	 */
	$sql = "SELECT * FROM cte.periodoreferencia";
	$periodosDeReferencia = $db->carregar($sql);
	$bloqueaSubacaoCronograma = "S";	
	
	for($i=0; $i<count($periodosDeReferencia); $i++){
		
		if($_REQUEST['perid'] == $periodosDeReferencia[$i]['perid'])
		{						
			if($subacaoindicador['mesinicio'] >= substr($periodosDeReferencia[$i]['perdtinicioref'],4,6) && $subacaoindicador['mesinicio'] <= substr($periodosDeReferencia[$i]['perdtterminoref'],4,6))
			{								
				$permissaogravar = "S";
				
			}elseif($subacaoindicador['mesfim'] <= substr($periodosDeReferencia[$i]['perdtterminoref'],4,6) && $subacaoindicador['mesfim'] >= substr($periodosDeReferencia[$i]['perdtinicioref'],4,6))
			{								
				$permissaogravar = "S";
				
			}elseif($subacaoindicador['anotermino'] > $_REQUEST['perano'] && substr($periodosDeReferencia[$i]['perdtinicioref'],4,6) >= $subacaoindicador['mesinicio'])
			{								
				$permissaogravar = "S";
								
			}else{
												
				$permissaogravar = "N";
				$bloqueaSubacaoCronograma = "N";
			}
		}
	}

	/***********************************************
	 * ALERTA MONITORAMENTO SE ESTIVER FORA DO CRONOGRAMA
	 */	
	if($bloqueaSubacaoCronograma == "N" && $execucaoSubacao){

		$removeSubacao = 	'						
							<tr>
								<td colspan="2" class="SubTituloDireita" style="background: #DB7093;"><center>Monitoramento fora do cronograma</center></td>
								<!-- 
								<td>
									<input type="button" value="Remover monitoramento" onclick="removeSubacao()"> 
								</td>
								-->
							</tr>
							';
		
		$permissaogravar = "S";		
	} elseif ($bloqueaSubacaoCronograma == "N") {
		
		$permissaogravar = "N";
	}

	/******************************************
	 * VERIFICA STATUS DA SUBAÇÃO 3 = APROVADA PELA COMISSÃO
	 */
	if($subacaoindicador['ssuid'] != '3' && $subacaoindicador['ssuid'] != null){
		
		$permissaogravar = "N";
		
		$sql = "
				select exeid from cte.execucaosubacao 
				where mntid = ".$_SESSION['cte_var']['mntid']."				 
				and exeatual = true
				and perid = '".$_REQUEST['perid']."' 
			   ";
				
		$sbaNaoAtendida = $db->pegaUm($sql);
				
		if($sbaNaoAtendida){
			
			$permissaogravar = "S";
		}
		
	}
	
	/********************************************************************
	 * REGRA PARA LIBERAR SUBAÇÃO QUANDO O ESTADO FOR
	 * NÃO INICIADO, EM ANDAMENTO, SUSPENSO
	 */
	$peridIndice = $_REQUEST['perid'] - 1;
	$sql = "
			select * from cte.execucaosubacao 
			where mntid = ".$_SESSION['cte_var']['mntid']." 
			and estid in (1, 2, 3) 
			and exeatual = true
			and perid = '".$peridIndice."' 
		   ";		
	$periodoNIniciado = $db->carregar($sql);	
	$liberaPeriodo = $periodoNIniciado[0]['perid'] + 1;
	
	if($periodoNIniciado)
	{				
		if($_REQUEST['perid'] == $liberaPeriodo)
		{			
			$permissaogravar = "S";			
		}
	}	
	
	/**************************************
	 * BLOQUEIA EDIÇÃO DA SUBAÇÃO PARA PRFIL
	 */	
	$perfis = cte_arrayPerfil();	
	if( ( count($perfis == 1)) && (in_array(CTE_PERFIL_CONSULTA_MUNICIPAL, $perfis ) )){
		//não altera
		$permissaogravar = 'N';
	}
		
?>
<form action="?modulo=principal/monitora_dados&acao=A" method="post" name="formulario" id="formulario">
	<input type="hidden" name="perid" value="<? echo $_REQUEST['perid']; ?>">
	<input type="hidden" name="mntidT" value="<? echo $_REQUEST['mntid']; ?>">
	<input type="hidden" name="requisicao" value="inserirexecucaosubacao">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<?//=$removeSubacao;?>
		<tr>
			<td class="SubTituloDireita" width="20%">Período de referência :</td>
			<td><b><? if( $foraDoPrazo ) echo $perdscselecionado; else echo $labelSemestre; ?></b></td>
			<!-- <td><b><? echo $labelSemestre; ?></b></td> -->
		</tr>
		<tr id="tr_acumuladoate">
			<td class="SubTituloDireita">Acumulado até :</td>
			<td>
				<?php
				if($permissaoperreferencia[$_REQUEST['perano']]['i']) {
					$mesini = $permissaoperreferencia[$_REQUEST['perano']]['i'];
				}
				if($permissaoperreferencia[$_REQUEST['perano']]['f']) {
					$mesfim = $permissaoperreferencia[$_REQUEST['perano']]['f'];
				}
				$sql = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses WHERE mescod >= '".$mesini."' AND  mescod <= '".$mesfim."'";
				
				$db->monta_combo('exedtexecucao_mes', $sql, $permissaogravar, "Selecione...", '', '', '', '120', 'N', 'exedtexecucao_mes');
				?>
				/ 
				<?php echo $_REQUEST['perano']; ?><input type="hidden" name="exedtexecucao_ano" id="exedtexecucao_ano" value="<? echo $_REQUEST['perano']; ?>">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Estado da subação :</td>
			<td>
				<?php
				if ($subacaoindicador['formaexecucao'] == "Executadas pelo município")
				{
					$sql = "SELECT estid AS codigo, estdsc AS descricao
							FROM cte.estadosubacao";
				} else {
					$sql = "SELECT estid AS codigo, estdsc AS descricao
							FROM cte.estadosubacao where estid != '4'";
				}
				
				$db->monta_combo('estid', $sql, $permissaogravar, "Selecione...", 'verificarmotivo', '', '', '130', 'N', 'estid');
				?>
				<input type="hidden" value="padrao" name="tipo_unidade_medida" id="tipo_unidade_medida" />
			</td>
		</tr>
		<tr id="tr_quantidade">
			<td class="SubTituloDireita"><input type="hidden" name="undid" id="undid" value="<?php echo $undid ?>" />Quantidade :</td>
			<td>
				<? echo campo_texto('exeqtd', 'S', $permissaogravar, '', 11, 10, '##########', '', 'left', '', 0, 'id="exeqtd"'); ?>
			</td>
		</tr>
		<?php
		$obPolo = new Polo();
		$arPolo = $obPolo->recuperarTodos( 'polid as codigo, polnome as descricao', array( "muncod = '$muncod'" ) );
		
		$obSituacaoCursista = new SituacaoCursista();
		$arSituacaoCursista = $obSituacaoCursista->recuperarTodos( 'scuid as codigo, scudsc as descricao' );		
				
		$sql = "SELECT sba.undid FROM cte.subacaoindicador sba
				INNER JOIN cte.unidademedida und
					ON sba.undid = und.undid
				WHERE sba.sbaid = '".$subac['sbaid']."'";
		$undidAnterior = $db->pegaUm($sql, 0);		
		
		$undid = $undid ? $undid : $undidAnterior;
		
		if( in_array( $undid, $arUnidades ) ){
			
			$arEscolas = recuperarEscolasPorMuncod( $muncod );
			
			$indice = count($arEscolas);
			$arEscolas[$indice]["inep"] = '99007410';
			$arEscolas[$indice]["codigo"] = '291572';
			$arEscolas[$indice]["descricao"] = 'SECRETARIA MUNICIPAL DE EDUCAÇÃO';
			$y = count($arEscolas);
		?>
		<tr>
			<td colspan="2" class="SubTituloDireita"><center>Informações</center></td>
		</tr>
		<tr id="tr_pessoa">
			<td colspan="2">
				<table id="tabelaEscolas" class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="100%" >
					<thead>
						<tr align="center">
							<td>&nbsp;</td>	
							<td>CPF</td>
							<td>Nome</td>
							<?php
							$unidadeSemPolo = 0;
							if( in_array( $undid, $arUnidadesSemPolo ) ){
								$unidadeSemPolo = 1;
							}
							 
							if( !$unidadeSemPolo ){
							?>
								<td>Polo</td>
								<td>Escola</td>
							<?php } ?>
								<td>Situação</td>	
						</tr>	
					</thead>
					<?php for( $i=0; $i<$exeqtd; $i++ ){
					
						$eesnome 		= "";
						$eescpf 		= "";
						$polid 			= "";
						$eesentescola 	= "";
						$eesentqtd 		= "";
						$scuid 			= "";
						
			  			if( $coEntidadeExecucaoSubacao[$i] ){
							$eesnome 		= $coEntidadeExecucaoSubacao[$i]["eesnome"];
							$eescpf 		= $coEntidadeExecucaoSubacao[$i]["eescpf"];
							$polid 			= $coEntidadeExecucaoSubacao[$i]["polid"];
							$eesentescola 	= $coEntidadeExecucaoSubacao[$i]["eesentescola"];
							$eesentqtd 		= $coEntidadeExecucaoSubacao[$i]["eesentq"];
							$scuid 			= $coEntidadeExecucaoSubacao[$i]["scuid"];
							
			  			} ?>
						<tr align="center" id="linha_<?php echo $i ?>" >
							<td>
								<img name="removeItem[]" onclick="removerLinha( 'linha_<?php echo $i ?>' )" src="/imagens/excluir.gif" title="Excluir" alt="Excluir" />
								<!-- <img name="removeItem[]" onclick="removerLinha( this.parentNode.parentNode.rowIndex )" src="/imagens/excluir.gif" title="Excluir" alt="Excluir" /> -->
							</td>
							<td>
								<input type="hidden" name="arees[]" value="<?php echo $i; ?>" />
			  					<? echo campo_texto("eescpf_$i", 'S', 'S', '', '18', '', '###.###.###-##', '', '', '', '', 'id="eescpf_'.$i.'" onchange="procurarNome( this.value, \''.$i.'\' )"', '', $eescpf, 'validarEescpf( this ), procurarNome( this.value, \''.$i.'\' )"'); ?>
							</td>
							<td>
								<? echo campo_texto("eesnome_$i", 'N', 'N', '', '30', '', '', '', '', '', '', 'id="eesnome_'.$i.'"', '', $eesnome); ?>
							</td>
							<?php if( !$unidadeSemPolo ){ ?>
								<td>
									<? echo $db->monta_combo("polid_$i", $arPolo,  'S', 'Selecione', '', '', '', '130', 'N', "polid_$i", '', $polid ); ?>
								</td>
								<td>
									<?php
									$html = "<select id=\"eesentescola_$i\" name=\"eesentescola_$i\" style=\"width: 240px\" class=\"CampoEstilo\">
									";
									for($x=0;$x<$y;$x++)
									{
										
										if ( $eesentescola == $arEscolas[$x]['codigo'] )
										{
											$sel = 'selected="selected"';
										} else {
											$sel = '';
										}
										
										$html .= "<option value='".$arEscolas[$x]['codigo']."' title='".$arEscolas[$x]['descricao']."' ".$sel.">".$arEscolas[$x]['inep']." - ".$arEscolas[$x]['descricao']."</option>";
									}
									$html .= "</select>";
									echo $html;
									?>
									<img src="../../imagens/obrig.gif">
								</td>
							<?php } ?>
								<td>
									<? echo $db->monta_combo("scuid_$i", $arSituacaoCursista,  'S', 'Selecione', '', '', '', '150', 'S', "scuid_$i", '', $scuid ); ?>
								</td>
							
						</tr>
					<?php }?>
				</table>
				
				<input type="button" value="Adicionar Linha" onclick="carregarQuantidade('<?php echo $unidadeSemPolo;?>')" />
				<script type="text/javascript"> alterarTipoUnidadeMedida( "pessoa" ); </script>
			</td>
		</tr> 
		<?php } elseif( $undid == CTE_UNIDADE_MEDIDA_UNIDADES_ESCOLARES_MUN || $undid == CTE_UNIDADE_MEDIDA_UNIDADES_ESCOLARES_EST  ){ ?>
		<tr id="tr_escolas">
			<td class="SubTituloDireita">
				Informações
				<script type="text/javascript"> alterarTipoUnidadeMedida( "escolas" ); </script>
			</td>
			<td>
				<table id="tabelaEscolas" class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="100%" >
					<thead>
						<tr>
							<td align="center" width="90%">Escola</td>
							<td align="center" width="5%">Qtd</td>
						</tr>	
			  			<?php if( is_array( $coEntidadeExecucaoSubacao ) ){
			  				
			  				unset( $_SESSION["arEntidadesSelecionadas"] );
			  				$arEntidadesSelecionadas = array();
							foreach( $coEntidadeExecucaoSubacao as $arEntidadeExecucaoSubacao ){
								$arEntidadesSelecionadas[] = $arEntidadeExecucaoSubacao["eesentescola"];
								$eesentescola = $arEntidadeExecucaoSubacao["eesentescola"]; ?>
								<tr style="background: #fff;" id="linha_<?php echo $eesentescola; ?>" >	
									<td>
										<input type="hidden" name="arees[]" value="<?php echo $eesentescola; ?>" />
										<input type="hidden" name="eesentescola_<?php echo $eesentescola; ?>" value="<?php echo $eesentescola; ?>" />
										
										<?php 
										$sql = " select entnome from entidade.entidade where  entid = $eesentescola";
										$arEntidade = $db->carregar( $sql );
										echo $arEntidade[0]["entnome"]; 
										?>
									</td>
									<td align="center">
										<?php echo campo_texto("eesentqtd_$eesentescola", 'N', 'S', '', '4', '', '', '', '', '', '', 'id="eesentqtd_'.$eesentescola.'"', '', $arEntidadeExecucaoSubacao["eesentqtd"] ); ?>
									</td>
								</tr>
				  			<?php } $_SESSION["arEntidadesSelecionadas"] = $arEntidadesSelecionadas; ?>
			  			<?php } ?>
					</thead>
					<tbody id="tbodyTabela">
					</tbody>
				</table>
				<br />
				<span id="addEntidadeExecucao" name="addEntidadeExecucao" style="cursor:pointer;display:block;float:left" onclick="popupSelecionarEscolas( <?php echo $muncod; ?> );">
					<img src="/imagens/gif_inclui.gif" align="top" style="border: none" />&nbsp;&nbsp;&nbsp;Inserir/Excluir Escola
				</span>
			</td>
		</tr>
		<?php } ?>
		<tr id="tr_observacoes">
			<td class="SubTituloDireita">Observações :</td>
			<td><? echo campo_textarea('exeobs', 'S', $permissaogravar, '', '100', '10', '500'); ?></td>
		</tr>
		<? 		
		if( ( $estidAtual != EST_CONCLUIDO ) && ( $estidAtual != EST_ANDAMENTO ) ){
		?>
		<tr id="tr_situacao">
			<td class="SubTituloDireita">Situação :</td>
			<td><?
			$sql = "SELECT corcod, corimgav, corsignificado FROM cte.cor WHERE corcod != 0 order by corcod";
			$cores = $db->carregar($sql);
			
			if($cores[0]) {
				foreach($cores as $cor) {
					echo "<input type=\"radio\" name=\"corcod\" value=\"".$cor['corcod']."\" ".(($corcod==$cor['corcod'])?'checked':'')." ".(($permissaogravar=='N')?'disabled':'')."><img src=\"../imagens/".$cor['corimgav']."\" onmouseover=\"return escape('".$cor['corsignificado']."');\">";
				}
			}
			?></td>			
		</tr>
		<?
		} else {
			if(!$perAnoTermino){
				$mostra = '';
			}else{
				$mostra = 'style="display: none;"';
			}
			
		?>
		<tr id="tr_situacao" <?php echo $mostra; ?> >
			<td class="SubTituloDireita">Situação :</td>
			<td>
				<?php
				$sql = "SELECT corcod, corimgav, corsignificado FROM cte.cor WHERE corcod != 0 order by corcod";
				$cores = $db->carregar($sql);
				
				if($cores[0]) {
					foreach($cores as $cor) {
						echo "<input type=\"radio\" name=\"corcod\" value=\"".$cor['corcod']."\" ".(($corcod==$cor['corcod'])?'checked':'')." ".(($permissaogravar=='N')?'disabled':'')."><img src=\"../imagens/".$cor['corimgav']."\" onmouseover=\"return escape('".$cor['corsignificado']."');\">";
					}
				}
				?>
			</td>
		</tr> 
		<?php } ?>
		<tr id="tr_motivos" style="display:none">
			<td colspan="2" id="td_motivos"></td>
		</tr>
		<? if($layoutmotivo) { ?>
		<script type="text/javascript">
			var tipoUnidadeMedida = document.getElementById('tipo_unidade_medida').value; 
			
			if( tipoUnidadeMedida == "pessoa" ){
				document.getElementById('tr_pessoa').style.display = 'none';
			}
			else{
				if( tipoUnidadeMedida == "escolas" ){
					document.getElementById('tr_escolas').style.display = 'none';
				}
			}			
			document.getElementById('tr_acumuladoate').style.display = 'none';
			document.getElementById('tr_quantidade').style.display = 'none';
			//document.getElementById('tr_observacoes').style.display = 'none';
			
			document.getElementById('tr_situacao').style.display = 'none';
			if( estid.value == '<?php echo EST_NAOINICIADA; ?>' ){
				document.getElementById('tr_motivos').style.display = '';
				ajaxatualizar('requisicao=carregarmotivosmonitoramento&exeid=<? echo $execucao['exeid']; ?>','td_motivos');
			}
			else{
				document.getElementById('tr_motivos').style.display = 'none';
			}
		</script>
		<? } ?>
		<tr style="background-color: #cccccc">
			<td colspan="2" align="center">
				<input onclick="return navegarForm('<?=$_REQUEST['perano']?>', '<?=$mntidAnterior?>');" type="button" name="btnAnterior" value="Anterior" id="btnAnterior" <?php echo !$mntidAnterior ? 'disabled="disabled"' : ''; ?>/>
				<input onclick="return validarexecucao(<?=$mntidAnterior ?>);" type="button" name="btnGrAnterior" value="Grava e anterior" id="btnGrAnterior" <?php echo !$mntidAnterior ? 'disabled="disabled"' : ''; ?><? echo (($permissaogravar=='N')?'disabled':''); ?>/>
				<input type="button" value="Gravar" id="btnsalvar" onclick="return validarexecucao(<?=$_REQUEST['mntid'] ?>);" <? echo (($permissaogravar=='N')?'disabled':''); ?>> 
				<input type="button" value="Fechar" onclick="fecharPopup();">
				<input onclick="return validarexecucao(<?=$mntidProximo ?>);" type="button" name="btnGrProximo" value="Grava e próxima" id="btnGrProximo"        <?php echo !$mntidProximo ? 'disabled="disabled"' : ''; ?><? echo (($permissaogravar=='N')?'disabled':''); ?>/>
				<input onclick="return navegarForm('<?=$_REQUEST['perano']?>', '<?=$mntidProximo?>');" type="button" name="btnProximo" value="Próxima" id="btnProximo" <?php echo !$mntidProximo ? 'disabled="disabled"' : ''; ?>/>
			</td>
		</tr>
		<tr>
			<td colspan="2">
				<?php
				// Selecionando o historico de execuções para determinado periodo de referencia
				$cabecalho = array("Autor", "Estado da subação", "Data de execução", "Quantidade", "Situação");
				$sql = "SELECT usu.usunome,  '<a onmouseover=\"return escape(\''||COALESCE(exs.exeobs,'')||'\');\">'||ess.estdsc||'</a>' as estdsc, SUBSTR(exs.exedtexecucao,5,2)||'/'||SUBSTR(exs.exedtexecucao,1,4) as exedtexecucao, exs.exeqtd, '<img src=\"../imagens/'||cor.corimgav||'\">'  FROM cte.execucaosubacao exs
						LEFT JOIN cte.cor AS cor ON cor.corcod = exs.corcod
						LEFT JOIN cte.estadosubacao AS ess ON ess.estid = exs.estid
						LEFT JOIN seguranca.usuario AS usu ON exs.usucpf = usu.usucpf 
						WHERE mntid='".$_SESSION['cte_var']['mntid']."' AND perid = '".$_REQUEST['perid']."'
						ORDER BY exedtinclusao DESC
						limit 1 -- é para trazer somente o último registro "; 
				
				$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N'); 
				?>
			</td>
		</tr>
	</table>
</form>
<br/><br/>
<script>
	function consultarversoes(aneid) {
		document.getElementById('adicionarversao_'+aneid).style.display = 'none';
		if(document.getElementById('consultar_'+aneid).style.display == 'none') {
			document.getElementById('consultar_'+aneid).style.display = '';
		} else {
			document.getElementById('consultar_'+aneid).style.display = 'none';
		}
	}
	
	function adicionarversao(aneid) {
		document.getElementById('consultar_'+aneid).style.display = 'none';
		if(document.getElementById('adicionarversao_'+aneid).style.display == 'none') {
			document.getElementById('adicionarversao_'+aneid).style.display = '';
		} else {
			document.getElementById('adicionarversao_'+aneid).style.display = 'none';
		}
	}
</script>
<form action="?modulo=principal/monitora_dados&acao=A"  enctype="multipart/form-data" method="post" name="formularioDocumento" id="formularioDocumento">
	<input type="hidden" id="requisicao" name="requisicao" value="inseriranexo">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td colspan="2" class="SubTituloDireita" style="text-align: center; font-weight: bold;">Documentos</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Arquivo :</td>
			<td><input type="file" name="arquivo" value="Arquivo" id="arquivo" <? echo (($permissaogravar=='N')?"disabled":""); ?>/></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo :</td>
			<td>
				<?php
				$sql = "SELECT tamid AS codigo, tamdsc AS descricao
						FROM cte.tipoanexomonitoramento";
				$db->monta_combo('tamid', $sql, $permissaogravar, "Selecione...", "document.getElementById('linhaDescricao').style.display = '' ;", '', '', '100', 'S', 'tamid');
				?>
			</td>
		</tr>
		<tr id="linhaDescricao" style="display: none">
			<td class="SubTituloDireita">Descrição :</td>
			<td><? echo campo_textarea('anedsc', 'N', $permissaogravar, '', '100', '5', '250'); ?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td colspan="2" align="center">
				<input type="button" id="btnsalvarDocumento" value="Anexar" onclick="return validardocumento();" <? echo (($permissaogravar=='N')?"disabled":""); ?>> 
				<input type="button" value="Fechar" onclick="fecharPopup();">
			</td>
		</tr>
	</table>
</form>
<br />
<?php
$sql = "SELECT anm.aneid, anm.anedsc, tam.tamdsc
		FROM cte.anexomonitoramento anm
		LEFT JOIN cte.tipoanexomonitoramento tam ON tam.tamid = anm.tamid
		WHERE mntid='".$_SESSION['cte_var']['mntid']."'";
$anexos = $db->carregar($sql);
	if($anexos[0]) {
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr style="background-color: #e0e0e0">
		<td style="font-weight:bold; text-align:center; width:5%">&nbsp;</td>
		<td style="font-weight:bold; text-align:center; width:70%">Instrumento Legal</td>
		<td style="font-weight:bold; text-align:center; width:25%">Tipo</td>
	</tr>
	<?php foreach($anexos as $anexo) { ?>
	<tr>
		<td style="font-weight:bold; text-align:center; width:10%">
		<img align="absmiddle" src="../imagens/consultar.gif" onmouseover="this.style.cursor='pointer'" onclick="consultarversoes('<? echo $anexo['aneid']; ?>')" title="Consultar anexos"/>
		<img align="absmiddle" src="../imagens/gif_inclui.gif" onmouseover="this.style.cursor='pointer'" onclick="adicionarversao('<?= $anexo['aneid'] ?>')" title="inserir arquivo"/>
		<img align="absmiddle" src="../imagens/excluir.gif" onmouseover="this.style.cursor='pointer'" onclick="Excluir('?modulo=principal/monitora_dados&acao=A&requisicao=removeranexo&aneid=<?= $anexo['aneid'] ?>','Deseja realmente excluir o anexo?');" title="excluir instrumento"/>
		</td>
		<td><? echo $anexo['anedsc']; ?></td>
		<td><? echo $anexo['tamdsc']; ?></td>
	</tr>
	<tr id="adicionarversao_<? echo $anexo['aneid']; ?>" style="display:none">
		<td style="text-align:center; width:10%">
			<img align="absmiddle" src="../imagens/seta_filho.gif"/>
		</td>
		<td colspan="2">
			<form action="?modulo=principal/monitora_dados&acao=A"  enctype="multipart/form-data" method="post" name="anexo<? echo $anexo['aneid']; ?>" id="anexo<? echo $anexo['aneid']; ?>">
				<input type="hidden" id="requisicao" name="requisicao" value="inserirversaoanexo">
				<input type="hidden" name="aneid" value="<? echo $anexo['aneid']; ?>">
				<input type="file" name="arquivo" <? echo (($permissaogravar=='N')?"disabled":""); ?>/> <input type="button" value="Salvar" onclick="this.disabled=true;document.getElementById('anexo<? echo $anexo['aneid']; ?>').submit();" <? echo (($permissaogravar=='N')?"disabled":""); ?>>
			</form>
		</td>
	</tr>
	<tr id="consultar_<? echo $anexo['aneid']; ?>" style="display:none">
		<td style="text-align:center; width:10%">
		<img align="absmiddle" src="../imagens/seta_filho.gif"/>
		</td>
		<td colspan="2"><? 
		$sqlanexo = "SELECT * FROM cte.versaoanexomonitoramento ver 
					 LEFT JOIN public.arquivo arq ON ver.arqid = arq.arqid 
					 WHERE aneid='".$anexo['aneid']."'";
		$arquivos = $db->carregar($sqlanexo);
		if($arquivos[0]) {
			foreach($arquivos as $arquivo) {
				//echo "<a href=\"javascript:void(0);\" onclick=\"return windowOpen('?modulo=principal/monitora_dados&acao=A&requisicao=downloadarquivo&arqid=".$arquivo['arqid']."','blank','height=5,width=5,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=yes');\"><img border=\"0\" src=\"../imagens/salvar.png\"> ".$arquivo['arqnome'].".".$arquivo['arqextensao']."</a><br />";
				echo "<a href=\"javascript:void(0);\" onclick=\"window.location='?modulo=principal/monitora_dados&acao=A&download=S&arqid={$arquivo['arqid']}'; \"><img border=\"0\" src=\"../imagens/salvar.png\"> ".$arquivo['arqnome'].".".$arquivo['arqextensao']."</a><br />";
			}
		}
		?></td>
	</tr>	
	<?php } ?>
</table>
<?
	}

}
/***************** 
 * FIM - ABA DE EXECUÇÃO 
 *****************/
	break;
/***************** 
 * ABA DE RESTRIÇÃO 
 *****************/
	case 'restricao':
?>
<form action="?modulo=principal/monitora_dados&acao=A" method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" value="inserirrestricaoexecucao">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Tipo de restrição :</td>
			<td>
				<?php
				$sql = "SELECT trscod AS codigo, trsdsc AS descricao 
						FROM public.tiporestricao
						WHERE trsstatus='A'
						ORDER BY trsdsc";
				$db->monta_combo('trscod', $sql, $permissaogravar, "Selecione...", '', '', '', '100', 'N','trscod');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição da Restrição :</td>
			<td><!--<? echo campo_textarea('resdsc', 'N', $permissaogravar, '', '100', '5', '500'); ?>-->
			<textarea rows="5" cols="100" name="resdsc" id ="resdsc"><?=$resdsc ?></textarea>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Providência :</td>
			<td>
			<!--<? echo campo_textarea('resprovidencia', 'N', $permissaogravar, '', '100', '5', '500');?>-->
			<textarea rows="5" cols="100" name="resprovidencia" id ="resprovidencia"><?=$resdsc ?></textarea></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td colspan="2" align="center"><input type="button" id="btnsalvar" value="Salvar" onclick="return validarrestricao();" <? echo (($permissaogravar=='N')?"disabled":""); ?>> <input type="button" value="Fechar" onclick="fecharPopup();"></td>
		</tr>
	</table>
</form>
<form action="?modulo=principal/monitora_dados&acao=A&_page=restricao" method="post" name="filtro" id="filtro">
	<input type="hidden" name="naosuperadas" id="naosuperadas" value="sim">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">	
		<tr>
			<td colspan="2"><input type="checkbox" onclick="if(this.checked){document.getElementById('naosuperadas').value='sim';document.getElementById('filtro').submit();}else{document.getElementById('naosuperadas').value='nao';document.getElementById('filtro').submit();}" <? echo (($_REQUEST['naosuperadas']=="sim")?"checked":""); ?>> Exibir apenas as restrições não superadas.</td>
		</tr>
	</table>
</form>
<? 
if($_REQUEST['naosuperadas']=="sim") {
	$filtrorestricaoexecucao = "AND ressuperada = false";
}

$sql = "SELECT rse.resid, ree.trsdsc, rse.resdsc, rse.resprovidencia, to_char(rse.resdtinclusao, 'dd/mm/YYYY HH24:MI:SS') as resdtinclusao, to_char(rse.resdtsuperacao, 'dd/mm/YYYY HH24:MI:SS') as resdtsuperacao, rse.ressuperada, usu2.usucpf as supercpf, usu2.usunome as autorsuperacao, usu1.usucpf as autorcpf, usu1.usunome as autorrestricao, usu1.usufoneddd as autorusufoneddd, usu2.usufoneddd as superusufoneddd, usu1.usufonenum as autorusufonenum, usu2.usufonenum as superusufonenum, uni1.unidsc as autorunidade, uni2.unidsc as superunidade FROM cte.restricaoexecucao rse 
		LEFT JOIN public.tiporestricao ree ON ree.trscod = rse.trscod
		LEFT JOIN seguranca.usuario usu1 ON usu1.usucpf = rse.usucpf 
		LEFT JOIN public.unidade uni1 ON uni1.unicod = usu1.unicod
		LEFT JOIN seguranca.usuario usu2 ON usu2.usucpf = rse.usucpfsuperacao 
		LEFT JOIN public.unidade uni2 ON uni2.unicod = usu2.unicod
		WHERE rse.mntid='".$_SESSION['cte_var']['mntid']."' ".$filtrorestricaoexecucao." 
		ORDER BY resdtinclusao DESC";
$restricoes = $db->carregar($sql);
if($restricoes[0]) {
	foreach($restricoes as $restricao) {
?>
<form action="?modulo=principal/monitora_dados&acao=A" method="post" id="<? echo "frm".$restricao['resid']; ?>">
	<input type="hidden" id="requisicao<? echo "frm".$restricao['resid']; ?>" name="requisicao" value="atualizarrestricaoexecucao">
	<input type="hidden" name="resid" value="<? echo $restricao['resid']; ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloEsquerda">&nbsp;</td>
			<td class="SubTituloEsquerda">Restrições</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" nowrap>Tipo de restrição :</td>
			<td><? echo $restricao['trsdsc']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição :</td>
			<td><? echo $restricao['resdsc']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data :</td>
			<td><? echo $restricao['resdtinclusao']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Autor :</td>
			<td>
			<img onclick="enviar_email( '<? echo $restricao['autorcpf']; ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/> <? echo $restricao['autorrestricao']; ?><br />
			<div style="color:#959595;"><? echo $restricao['autorunidade']; ?> - Tel: (<? echo $restricao['autorusufoneddd']; ?>) <? echo $restricao['autorusufonenum']; ?></div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Restrição superada?</td>
			<td>
			<input type="radio" name="ressuperada" value="sim" <? echo (($restricao['ressuperada']=="t")?'checked':''); ?> <? echo (($permissaogravar=='N')?"disabled":""); ?>> Sim <input type="radio" name="ressuperada" value="nao" <? echo (($restricao['ressuperada']=="f")?'checked':''); ?> <? echo (($permissaogravar=='N')?"disabled":""); ?>> Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Providência :</td>
			<td><? $resprovidencia= $restricao['resprovidencia']; echo campo_textarea('resprovidencia', 'N', $permissaogravar, '', '100', '5', '500'); ?></td>
		</tr>
		<?php if($restricao['ressuperada']=="t") { ?>
		<tr>
			<td class="SubTituloEsquerda">&nbsp;</td>
			<td class="SubTituloEsquerda">Superação</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data :</td>
			<td><? echo $restricao['resdtsuperacao']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Autor :</td>
			<td>
			<img onclick="enviar_email( '<? echo $restricao['supercpf']; ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/> <? echo $restricao['autorsuperacao']; ?><br />
			<div style="color:#959595;"><? echo $restricao['superunidade']; ?> - Tel: (<? echo $restricao['superusufoneddd']; ?>) <? echo $restricao['superusufonenum']; ?></div>
			</td>
		</tr>
		<?php } ?>
		<tr style="background-color: #cccccc">
			<td colspan="2" align="center"><input type="button" value="Salvar" onclick="this.disabled=true;document.getElementById('<? echo "frm".$restricao['resid']; ?>').submit();" <? echo (($permissaogravar=='N')?"disabled":""); ?>> <input type="button" value="Excluir" onclick="this.disabled=true;document.getElementById('requisicao<? echo "frm".$restricao['resid']; ?>').value='removerrestricaoexecucao';document.getElementById('<? echo "frm".$restricao['resid']; ?>').submit();" <? echo (($permissaogravar=='N')?"disabled":""); ?>></td>
		</tr>
	</table> 
</form>
<?
		}
	}
/***************** 
 * FIM - ABA DE RESTRIÇÃO 
 *****************/
	break;
	case 'observacoes':
	if(count($permissaoperiodocadastramento) > 0) {
		$permissaogravar = 'S';
	} else {
		$permissaogravar = 'N';
	}
?>
<form action="?modulo=principal/monitora_dados&acao=A" method="post" name="formulario" id="formulario">
	<input type="hidden" id="requisicao" name="requisicao" value="inserirobservacao">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Observação :</td>
			<td><? echo campo_textarea('obsdsc', 'N', $permissaogravar, '', '100', '5', '250'); ?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td colspan="2" align="center"><input type="button" value="Salvar" onclick="this.disabled=true;document.getElementById('formulario').submit();" <? echo (($permissaogravar=='N')?"disabled":""); ?>> <input type="button" value="Fechar" onclick="window.close();"></td>
		</tr>
	</table>
</form>
<br />
<?php
$sql = "SELECT obsid, obsdsc, to_char(obsdtinclusao, 'dd/mm/YYYY HH24:MI:SS') as obsdtinclusao, usu.usucpf, usu.usunome, usu.usufoneddd, usufonenum, uni.unidsc 
		FROM cte.observacaomonitoramento obm
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = obm.usucpf
		LEFT JOIN public.unidade uni ON uni.unicod = usu.unicod
		WHERE mntid='".$_SESSION['cte_var']['mntid']."' AND obsstatus='A'";
$observacoes = $db->carregar($sql);
if($observacoes) {
	foreach($observacoes as $obs) {
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td class="SubTituloEsquerda">&nbsp;</td>
		<td class="SubTituloEsquerda">Observações</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" nowrap>Descrição :</td>
		<td><? echo $obs['obsdsc']; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data :</td>
		<td><? echo $obs['obsdtinclusao']; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Autor :</td>
		<td>
		<img onclick="enviar_email( '<? echo $obs['usucpf']; ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/> <? echo $obs['usunome']; ?><br />
		<div style="color:#959595;"><? echo $obs['unidsc']; ?> - Tel: (<? echo $obs['usufoneddd']; ?>) <? echo $obs['usufonenum']; ?></div>
		</td>
	</tr>
</table>
<form action="?modulo=principal/monitora_dados&acao=A" method="post" name="excluir" id="excluir<? echo $obs['obsid']; ?>">
	<input type="hidden" id="requisicao" name="requisicao" value="removerobservacao">
	<input type="hidden" name="obsid" value="<? echo $obs['obsid']; ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr style="background-color: #cccccc">
			<td colspan="2" align="center"><input type="button" value="Excluir" onclick="ExcluirPorSubmit('excluir<? echo $obs['obsid']; ?>','Deseja realmente excluir a observação?');" <? echo (($permissaogravar=='N')?"disabled":""); ?>></td>
		</tr>
	</table>
</form>
<?
		}
	}
	break;
}
?>
<?php if(!$_REQUEST['perid']) { ?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td>
			<center>	
			<input type="button" value="Fechar" onclick="fecharPopup();">	
			</center>
		</td>
	</tr>
</table>
<?php } ?>
<script language="JavaScript" src="includes/wz_tooltip.js"></script> 
</body>
</html>