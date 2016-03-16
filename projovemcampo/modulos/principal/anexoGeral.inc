<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
	ob_clean();
    $arquivo = $file->getDownloadArquivo($arqid);
    exit;
} 

if( $_FILES["arquivo"] ){
	$campos = array("angdsc" => "'".$_POST['descricao']."'");
	$file = new FilesSimec("anexogeral", $campos ,"projovemcampo");
	$arquivoSalvo = $file->setUpload($_POST['descricao'], '', true);
	
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
		echo"<script>window.location.href = 'projovemcampo.php?modulo=principal/anexoGeral&acao=A';</script>";
		die;	
	}
}

if($_GET['arqidDel']){
    $sql = "DELETE FROM projovemcampo.anexogeral WHERE arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    
    $db->commit();
    
    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);
	
    echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href="projovemcampo.php?modulo=principal/anexoGeral&acao=A";
    	  </script>';
    die;
}




// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

$titulo = "Projovem Urbano";
$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';

echo "<br>";

$perfis = pegaPerfilGeral();

if(in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)|| in_array(PFL_ADMINISTRADOR, $perfis)) {
	$menu = array(0 => array("id" => 1, "descricao" => "Painel", 			"link" => "/projovemcampo/projovemcampo.php?modulo=inicio&acao=C"),
				  1 => array("id" => 2, "descricao" => "Documentos", 	"link" => "/projovemcampo/projovemcampo.php?modulo=principal/anexoGeral&acao=A")
			  	  );
}
else{
	$menu = array(0 => array("id" => 1, "descricao" => "Documentos", 	"link" => "/projovemcampo/projovemcampo.php?modulo=principal/anexoGeral&acao=A")
			  	  );
}


echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);


monta_titulo( $titulo, $dsctitulo );
echo "<br>";

$db->cria_aba( $abacod_tela, $url, '');


//pega array de perfis
//$arPerfil = pegaPerfil( $_SESSION['usucpf'] );

				
if(in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)|| in_array(PFL_ADMINISTRADOR, $perfis)) {
?>
	<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
	<script language="javascript" type="text/javascript">

	/**
	 * Submete o formulário
	 * @name enviaArquivos
	 * @return void
	 */
	function enviaArquivos( arquivo ){
	   var formulario = $('formulario');
	   meuerro = "";
	   if (!arquivo) {
	      //Se não tenho arquivo, é porque não se selecionou um arquivo no formulário.
	        meuerro = "Por favor, selecionar o arquivo que deseja anexar para realizar essa ação.";
	   }else{
   		   	formulario.submit();
	   }
	   //se estou aqui é porque não se pode submeter
	   //alert (meuerro);
		   //return 0;
		}
		
		function excluirAnexo( arqid ){
		 	if ( confirm( 'Deseja excluir este Documento?' ) ) {
		 		location.href= window.location+'&arqidDel='+arqid;
		 	}
		 }
		 
	</script>
	<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
	    	<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
	    		<tr>
	    			<td class="SubtituloDireita" style="width: 33%">Arquivo:</td>
	    			<td><input type="file" name="arquivo" id="arquivo" /></td>
	    		</tr>
	    		<tr>
	    			<td class="SubtituloDireita" style="width: 33%">Descrição:</td>
	    			<td>
	    				<?=campo_textarea("descricao","N","S","", 60, 3, 255)?>
					</td>
	    		</tr>
	    		<tr>
	    			<td class="SubtituloDireita" style="width: 33%">&nbsp;</td>
	    			<td class="SubtituloEsquerda">
	    				<input type="button" name="botao" class="botao" value="Enviar" onclick="enviaArquivos(this.form.arquivo.value);"/>
	    			</td>
	    		</tr>
	
	    	</table>
	</form>
<?
$btnExcluir = "<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirAnexo(\''||arq.arqid||'\');\">";

} // FECHA 

if( !$db->testa_superuser() && !in_array(PFL_ADMINISTRADOR, $perfis) && !in_array(PFL_SECRETARIO_MUNICIPAL, $perfis) && !in_array(PFL_SECRETARIO_ESTADUAL, $perfis)
	 && !in_array(PFL_COORDENADOR_ESTADUAL, $perfis) && !in_array(PFL_COORDENADOR_MUNICIPAL, $perfis) && !in_array(PFL_DIRETOR_POLO, $perfis) && !in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
	$whereUSU = "AND usu.usucpf = '{$_SESSION['usucpf']}'";
}
	
$sql = "SELECT 
			'<center><a href=\"projovemcampo.php?modulo=principal/anexoGeral&acao=A&download=S&arqid='|| arq.arqid ||'\"><img src=../imagens/anexo.gif></a>&nbsp; $btnExcluir</center>' as acoes, 
			'<a href=\"projovemcampo.php?modulo=principal/anexoGeral&acao=A&download=S&arqid='|| arq.arqid ||'\">' || arq.arqnome || '.' || arq.arqextensao || '</a>' as arquivo, 
			arq.arqdescricao, 
			arq.arqtamanho,
			to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora as arqdata,  
			usu.usunome
		FROM 
			projovemcampo.anexogeral con
		INNER JOIN public.arquivo arq on con.arqid = arq.arqid
        INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf 
        WHERE 
        	con.angtip is null
			$whereUSU";

//dbg($sql,1);
$cabecalho = array( "Ações","Arquivo" , "Descrição", "Tamanho(bytes)", "Data inclusão", "Responsável");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');

?>
