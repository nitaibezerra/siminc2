<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

// Verifica se o 'memid' existe na sessão.
if(!$_SESSION['memid']) {
	echo "<script type=\"text/javascript\">
			alert(\"O cadastro de Diretor e Coordenador deve ser realizado para acessar esta aba.\");
			location.href = \"pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A\";
		  </script>";
	exit;
}

include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
me_verificaSessao();

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"];
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = meCriarDocumento( $_SESSION['meentid'] , $_SESSION['memid']  );
	if($docid == false) {
		echo "<script>
				alert('Esta entidade não consta na tabela principal do Mais Educação');
				history.back(-1);
			  </script>";
		die;
	}
}

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod in (".PDEESC_PERFIL_CAD_MAIS_EDUCACAO.", ".PDEESC_PERFIL_SUPER_USUARIO.")";
$vPerfilCadMaisEscola = $db->pegaUm($sql);

if((integer)$vPerfilCadMaisEscola > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == meMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = meCriarDocumento( $_SESSION['meentid'], $_SESSION['memid']   );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Mais Educação');
						history.back(-1);
					  </script>";
				die;
			}
		} else {
			$docid = false;
		}
	}

	$esdid = mePegarEstadoAtual( $docid );
	
	if((integer)$esdid == CADASTRAMENTO_ME || (integer)$esdid == CORRECAO_CADASTRAMENTO_ME) {
		$somenteConsulta = false;
	}
}

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec("mearquivos", $campos  );
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'pdeescola.php?modulo=meprincipal/documentos_anexos&acao=A';</script>";
    exit;
}   
echo "<br />";
echo montarAbasArray(carregaAbasMaisEducacao(), "pdeescola.php?modulo=meprincipal/documentos_anexos&acao=A");

$titulo = "Mais Educação - Educação Integral";
//$subtitulo = "Cadastro - Atividades ".((integer)$_SESSION["exercicio"] - 1) . '<br> <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>  Indica Campo Obrigatório';

monta_titulo($titulo, $subtitulo);

echo cabecalhoMaisEducacao();
 
$campos	= array("memid"		=> $_SESSION['memid'],
				"arpdata"	=>"'".date('Y-m-d')."'",
				"arpstatus" => "'A'"
				);	
$file = new FilesSimec("mearquivos", $campos ,"pdeescola");

if($_FILES["Arquivo"]){	
	//pre( $_POST ,1);
	$arquivoSalvo = $file->setUpload( "".$_POST['arqdescricao']."");
	if($arquivoSalvo){
		echo '<script type="text/javascript">alert("Operação efetuada com sucesso.");</script>';
		echo"<script>window.location.href = 'pdeescola.php?modulo=meprincipal/documentos_anexos&acao=A';</script>";	
	}
}
 
if($_REQUEST['arqidDel'] != ''){
	
	//dbg($_REQUEST['arqidDel'],1);
    
    $sql = "UPDATE pdeescola.mearquivos SET arpstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    
    //exlcui arquivo fisico
    $file->excluiArquivoFisico($_REQUEST['arqidDel']);

    $db->commit();
    echo '<script type="text/javascript">alert("Operação efetuada com sucesso.");</script>';
    echo"<script>window.location.href = 'pdeescola.php?modulo=meprincipal/documentos_anexos&acao=A';</script>";

}
 
?> 
<form name=formulario id=formulario method=post enctype="multipart/form-data">
<input type="hidden" name="salvar" value="0"> 
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td colspan="2" align="center">
			<br>
			<b>
			Esta aba é destinada à inserção de fotos e arquivos que mostrem os diferentes espaços (dentro e fora da escola) onde ocorrem as atividades do Programa Mais Educação.
			</b>
			<br>
			<br>
		</td>
	</tr>
	<tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Fotos e arquivos:</td>
        <td width='50%'>
            <input type="file" name="Arquivo" id="Arquivo"/>
            <!-- 
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
             -->
        </td>      
    </tr>
    <!--<tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
        <td><?php 
        /*
          $sql = "
            SELECT tpaid AS codigo, tpadescricao AS descricao
                FROM evento.tipoanexo
        "; 
        $db->monta_combo('tpaid', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'tpaid'); 
		*/
        ?></td>
    </tr>
    -->
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
        <td><?= campo_textarea( 'arqdescricao', 'N', 'S', '', 60, 2, 250 ); ?></td>
    </tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
        <td height="30">
        	<? if(!$somenteConsulta) { ?>
        		<input type="button" name="botao" value="Salvar" onclick="validaForm();">
        	<? } ?>
        </td>
    </tr> 
</table>
<table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
    <?
    	if( $_SESSION['memid'] != '' ){
	        $sql = "SELECT
	                        '<center><a href=\"#\" onclick=\"javascript:excluirAnexo(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,                       
	                        to_char(arq.arqdata,'DD/MM/YYYY'),
	                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/documentosAnexos&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
	                        arq.arqtamanho || ' kbs' as tamanho ,
	                        arq.arqdescricao
	                    FROM public.arquivo arq 
	                         INNER JOIN pdeescola.mearquivos aqb ON arq.arqid = aqb.arqid  
	                    WHERE
	                        aqb.arpstatus = 'A' AND  aqb.memid = ".$_SESSION['memid'];
	
	        $cabecalho = array( "Ação",
	                            "Data Inclusão",
	                            "Nome Arquivo",
	                            "Tamanho (Mb)",
	                            "Descrição Arquivo");
	         
	        $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
    	}
    ?>
</table>


 
<script>
var query;
var objForm = document.forms["formulario"];

function validaForm(){
	var saida	= true;
	var alerta	= "Ocorreram os seguintes erros:\r\n\r\n";
	if(objForm.Arquivo.value==''){
		alerta	= alerta+" - Voce deve escolher um arquivo\r\n";
		saida	= false;
	}

	if(saida==false){
		alert(alerta);
	}
	else{
		objForm.submit();
	}
}
	function excluirAnexo( arqid ){  
		if ( confirm( 'Deseja excluir o Documento?' ) ) {
			location.href= window.location+'&arqidDel='+arqid;
		}
	}

	function popupMapa(entid){
		window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	}
</script>