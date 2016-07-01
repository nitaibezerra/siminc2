<?php

checkAno();

ini_set("memory_limit", "128M");

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

function retornaAbaMestre()
{
    $acao      = $_REQUEST['acao'];
    $abaMestre = $_REQUEST['abaMestre'];
	
    if( empty($acao) || $acao == 'A' ){
        $retorno = "A"; // A = Orientaçaões
        
    }elseif( $abaMestre == "I" || $acao == 'I' ){
        $retorno = "I"; // I = Unidade Formativa I
        
    }elseif( $abaMestre == "J" || $acao == 'J' ){
        $retorno = "J"; // J = Unidade Formativa II
        
    }elseif( $abaMestre == "K" || $acao == 'K' ){
        $retorno = "K"; // K = Unidade Formativa III 
        
    }elseif( $abaMestre == "Y" || $acao == 'Y' ){
        $retorno = "Y"; // Y = Unidade Formativa IV
        
    }elseif( $abaMestre == "T" || $acao == 'T' ){
        $retorno = "T"; // T = Unidade Formativa V
        
    }elseif( $abaMestre == "Z" || $acao == 'Z' ){
        $retorno = "Z"; // Z = Unidade Formativa VI        
    }

    return $retorno;
}

function retornaAcao()
{
    $acao = $_REQUEST['acao'];

    if( empty($acao) ){
        $retorno = "A"; 
    }elseif( $acao == "I" || $acao == 'J' || $acao == 'K' || $acao == 'Y' || $acao == 'T' || $acao == 'Z' ){ //Retorna a Aba a ser selecionada
        $retorno = "B";
    }else{
        $retorno = $acao;
    }

    return $retorno;
}

$acao                   = retornaAcao();
$abaMestre              = retornaAbaMestre();
$tipoArquivo            = '';
$nomeTipoAba            = '';
$txUnidadeFormativaI    = 'Unidade Formativa I';
$txUnidadeFormativaII   = 'Unidade Formativa II';
$txUnidadeFormativaIII  = 'Unidade Formativa III';
$txUnidadeFormativaIV   = 'Unidade Formativa IV';
$txUnidadeFormativaV    = 'Unidade Formativa V';
$txUnidadeFormativaVI   = 'Unidade Formativa VI';
/*
 * Seleciona o tipo do anexo, informaçoes na tabela projovemurbano.anexogeral
 */
switch (true)
{
	case $acao == 'A' :                      $tipoArquivo = '1'; 	$nomeTipoAba = 'Orientações - Nota Técnica nº 85/2012'; $nomeAbaPai = $nomeTipoAba;           break;
	case $acao == 'B' && $abaMestre == 'I' : $tipoArquivo = '2'; 	$nomeTipoAba = 'Ciências da Natureza';                  $nomeAbaPai = $txUnidadeFormativaI;   break;
	case $acao == 'C' && $abaMestre == 'I' : $tipoArquivo = '3'; 	$nomeTipoAba = 'Ciências Humanas';                      $nomeAbaPai = $txUnidadeFormativaI;   break;
	case $acao == 'D' && $abaMestre == 'I' : $tipoArquivo = '4'; 	$nomeTipoAba = 'Inglês';                                $nomeAbaPai = $txUnidadeFormativaI;   break;
	case $acao == 'E' && $abaMestre == 'I' : $tipoArquivo = '5'; 	$nomeTipoAba = 'Língua Portuguesa';                     $nomeAbaPai = $txUnidadeFormativaI;   break;
	case $acao == 'F' && $abaMestre == 'I' : $tipoArquivo = '6'; 	$nomeTipoAba = 'Matemática';                            $nomeAbaPai = $txUnidadeFormativaI;   break;
	case $acao == 'G' && $abaMestre == 'I' : $tipoArquivo = '7'; 	$nomeTipoAba = 'Participação Cidadã';                   $nomeAbaPai = $txUnidadeFormativaI;   break;
	case $acao == 'H' && $abaMestre == 'I' : $tipoArquivo = '8'; 	$nomeTipoAba = 'Qualificação Profissional';             $nomeAbaPai = $txUnidadeFormativaI;   break;
	
	// Unidade Formativa II
	case $acao == 'B' && $abaMestre == 'J' : $tipoArquivo = '2A'; 	$nomeTipoAba = 'Ciências da Natureza';                  $nomeAbaPai = $txUnidadeFormativaII;   break;
	case $acao == 'C' && $abaMestre == 'J' : $tipoArquivo = '2B'; 	$nomeTipoAba = 'Ciências Humanas';                      $nomeAbaPai = $txUnidadeFormativaII;   break;
	case $acao == 'D' && $abaMestre == 'J' : $tipoArquivo = '2C'; 	$nomeTipoAba = 'Inglês';                                $nomeAbaPai = $txUnidadeFormativaII;   break;
	case $acao == 'E' && $abaMestre == 'J' : $tipoArquivo = '2D'; 	$nomeTipoAba = 'Língua Portuguesa';                     $nomeAbaPai = $txUnidadeFormativaII;   break;
	case $acao == 'F' && $abaMestre == 'J' : $tipoArquivo = '2E'; 	$nomeTipoAba = 'Matemática';                            $nomeAbaPai = $txUnidadeFormativaII;   break;
	case $acao == 'G' && $abaMestre == 'J' : $tipoArquivo = '2F'; 	$nomeTipoAba = 'Participação Cidadã';                   $nomeAbaPai = $txUnidadeFormativaII;   break;
	case $acao == 'H' && $abaMestre == 'J' : $tipoArquivo = '2G'; 	$nomeTipoAba = 'Qualificação Profissional';             $nomeAbaPai = $txUnidadeFormativaII;   break;

 	// Unidade Formativa III
 	case $acao == 'B' && $abaMestre == 'K' : $tipoArquivo = '3A'; 	$nomeTipoAba = 'Ciências da Natureza';                  $nomeAbaPai = $txUnidadeFormativaIII;   break;
 	case $acao == 'C' && $abaMestre == 'K' : $tipoArquivo = '3B'; 	$nomeTipoAba = 'Ciências Humanas';                      $nomeAbaPai = $txUnidadeFormativaIII;   break;
 	case $acao == 'D' && $abaMestre == 'K' : $tipoArquivo = '3C'; 	$nomeTipoAba = 'Inglês';                                $nomeAbaPai = $txUnidadeFormativaIII;   break;
 	case $acao == 'E' && $abaMestre == 'K' : $tipoArquivo = '3D'; 	$nomeTipoAba = 'Língua Portuguesa';                     $nomeAbaPai = $txUnidadeFormativaIII;   break;
 	case $acao == 'F' && $abaMestre == 'K' : $tipoArquivo = '3E'; 	$nomeTipoAba = 'Matemática';                            $nomeAbaPai = $txUnidadeFormativaIII;   break;
 	case $acao == 'G' && $abaMestre == 'K' : $tipoArquivo = '3F'; 	$nomeTipoAba = 'Participação Cidadã';                   $nomeAbaPai = $txUnidadeFormativaIII;   break;
 	case $acao == 'H' && $abaMestre == 'K' : $tipoArquivo = '3G'; 	$nomeTipoAba = 'Qualificação Profissional';             $nomeAbaPai = $txUnidadeFormativaIII;   break;
 	
 	// Unidade Formativa IV
 	case $acao == 'B' && $abaMestre == 'Y' : $tipoArquivo = '4A'; 	$nomeTipoAba = 'Ciências da Natureza';                  $nomeAbaPai = $txUnidadeFormativaIV;   break;
 	case $acao == 'C' && $abaMestre == 'Y' : $tipoArquivo = '4B'; 	$nomeTipoAba = 'Ciências Humanas';                      $nomeAbaPai = $txUnidadeFormativaIV;   break;
 	case $acao == 'D' && $abaMestre == 'Y' : $tipoArquivo = '4C'; 	$nomeTipoAba = 'Inglês';                                $nomeAbaPai = $txUnidadeFormativaIV;   break;
 	case $acao == 'E' && $abaMestre == 'Y' : $tipoArquivo = '4D'; 	$nomeTipoAba = 'Língua Portuguesa';                     $nomeAbaPai = $txUnidadeFormativaIV;   break;
 	case $acao == 'F' && $abaMestre == 'Y' : $tipoArquivo = '4E'; 	$nomeTipoAba = 'Matemática';                            $nomeAbaPai = $txUnidadeFormativaIV;   break;
 	case $acao == 'G' && $abaMestre == 'Y' : $tipoArquivo = '4F'; 	$nomeTipoAba = 'Participação Cidadã';                   $nomeAbaPai = $txUnidadeFormativaIV;   break;
 	case $acao == 'H' && $abaMestre == 'Y' : $tipoArquivo = '4G'; 	$nomeTipoAba = 'Qualificação Profissional';             $nomeAbaPai = $txUnidadeFormativaIV;   break;

 	// Unidade Formativa V
 	case $acao == 'B' && $abaMestre == 'T' : $tipoArquivo = '5A'; 	$nomeTipoAba = 'Ciências da Natureza';                  $nomeAbaPai = $txUnidadeFormativaV;   break;
 	case $acao == 'C' && $abaMestre == 'T' : $tipoArquivo = '5B'; 	$nomeTipoAba = 'Ciências Humanas';                      $nomeAbaPai = $txUnidadeFormativaV;   break;
 	case $acao == 'D' && $abaMestre == 'T' : $tipoArquivo = '5C'; 	$nomeTipoAba = 'Inglês';                                $nomeAbaPai = $txUnidadeFormativaV;   break;
 	case $acao == 'E' && $abaMestre == 'T' : $tipoArquivo = '5D'; 	$nomeTipoAba = 'Língua Portuguesa';                     $nomeAbaPai = $txUnidadeFormativaV;   break;
 	case $acao == 'F' && $abaMestre == 'T' : $tipoArquivo = '5E'; 	$nomeTipoAba = 'Matemática';                            $nomeAbaPai = $txUnidadeFormativaV;   break;
 	case $acao == 'G' && $abaMestre == 'T' : $tipoArquivo = '5F'; 	$nomeTipoAba = 'Participação Cidadã';                   $nomeAbaPai = $txUnidadeFormativaV;   break;
 	case $acao == 'H' && $abaMestre == 'T' : $tipoArquivo = '5G'; 	$nomeTipoAba = 'Qualificação Profissional';             $nomeAbaPai = $txUnidadeFormativaV;   break;
 	
 	// Unidade Formativa V
 	case $acao == 'B' && $abaMestre == 'Z' : $tipoArquivo = '6A'; 	$nomeTipoAba = 'Ciências da Natureza';                  $nomeAbaPai = $txUnidadeFormativaVI;   break;
 	case $acao == 'C' && $abaMestre == 'Z' : $tipoArquivo = '6B'; 	$nomeTipoAba = 'Ciências Humanas';                      $nomeAbaPai = $txUnidadeFormativaVI;   break;
 	case $acao == 'D' && $abaMestre == 'Z' : $tipoArquivo = '6C'; 	$nomeTipoAba = 'Inglês';                                $nomeAbaPai = $txUnidadeFormativaVI;   break;
 	case $acao == 'E' && $abaMestre == 'Z' : $tipoArquivo = '6D'; 	$nomeTipoAba = 'Língua Portuguesa';                     $nomeAbaPai = $txUnidadeFormativaVI;   break;
 	case $acao == 'F' && $abaMestre == 'Z' : $tipoArquivo = '6E'; 	$nomeTipoAba = 'Matemática';                            $nomeAbaPai = $txUnidadeFormativaVI;   break;
 	case $acao == 'G' && $abaMestre == 'Z' : $tipoArquivo = '6F'; 	$nomeTipoAba = 'Participação Cidadã';                   $nomeAbaPai = $txUnidadeFormativaVI;   break;
 	case $acao == 'H' && $abaMestre == 'Z' : $tipoArquivo = '6G'; 	$nomeTipoAba = 'Qualificação Profissional';             $nomeAbaPai = $txUnidadeFormativaVI;   break;
 	
	//O $tipoArquivo=2 porque devera carregar os dados de Ciencias da Natureza 
	case $acao == 'I': $tipoArquivo = '2'; 	    $nomeTipoAba = $txUnidadeFormativaI;                   $nomeAbaPai = $txUnidadeFormativaI;   break;
    case $acao == 'J': $tipoArquivo = '2B'; 	$nomeTipoAba = $txUnidadeFormativaII;                  $nomeAbaPai = $txUnidadeFormativaII;  break;
    case $acao == 'K': $tipoArquivo = '3B'; 	$nomeTipoAba = $txUnidadeFormativaIII;                 $nomeAbaPai = $txUnidadeFormativaIII; break;
    case $acao == 'Y': $tipoArquivo = '4B'; 	$nomeTipoAba = $txUnidadeFormativaIV;                  $nomeAbaPai = $txUnidadeFormativaIV;  break;
    case $acao == 'T': $tipoArquivo = '5B'; 	$nomeTipoAba = $txUnidadeFormativaV;                   $nomeAbaPai = $txUnidadeFormativaV;   break;
    case $acao == 'Z': $tipoArquivo = '6B'; 	$nomeTipoAba = $txUnidadeFormativaVI;                  $nomeAbaPai = $txUnidadeFormativaVI;  break;
}

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
	ob_clean();
    $arquivo = $file->getDownloadArquivo($arqid);
    exit;
} 
// ver($_SESSION['projovemurbano']['abaMestre']);
if( $_FILES["arquivo"] ){
	
	$campos = array(
		"angdsc" => "'".$_POST['descricao']."'",
		"angtip" => "'".$_POST['angtip']."'",
		"ppuid"  => $_SESSION['projovemurbano']['ppuid']
	);

	$file = new FilesSimec("anexogeral", $campos ,"projovemurbano");
	$arquivoSalvo = $file->setUpload($_POST['descricao'], '', true);
	
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
		echo"<script>window.location.href = 'projovemurbano.php?modulo=principal/bancoDeQuestoes&acao=".$acao."&abaMestre=".$abaMestre."';</script>";
		die;	
	}
}

if($_GET['arqidDel']){
    $sql = "DELETE FROM projovemurbano.anexogeral where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    
    $db->commit();
    
    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);
	
    echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href="projovemurbano.php?modulo=principal/bancoDeQuestoes&acao='.$acao.'&abaMestre='.$abaMestre.'";
    	  </script>';
    die;
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';

$titulo = "Projovem Urbano" . ' - ' . $nomeAbaPai;
$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';

echo "<br>";

$perfis = pegaPerfilGeral();

//utilizado nos titulos de páginas
$db->cria_aba( '57586', $url, '');
monta_titulo( $titulo, $dsctitulo );

// Inclusão da biblioteca jQuery
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="../projovemurbano/js/banco_de_questoes.js"></script>';
?>
<table width="100%">
    <tr>
        <td>&nbsp;</td>
    </tr>
</table>
<script type="text/javascript" language="javascript" >
    $(document).ready(function(){
    	BancoDeQuestoes.init();
    });
</script>
<?php 
/*
 * Se a Aba principal for I, J, K, Y, T ou Z, entao carregar Abas filhas
 */
// $abaMestreSessao = $_SESSION['projovemurbano']['abaMestre'];
// unset($_SESSION['projovemurbano']['abaMestre']);

if( $abaMestre == "I" || $abaMestre == "J" || $abaMestre == "K" || $abaMestre == "Y" || $abaMestre == "T" || $abaMestre == "Z" ){
    $titulo = "Projovem Urbano" . ' - ' . $nomeTipoAba;
    $db->cria_aba( '57585', $url, '');
    monta_titulo( $titulo, '' );
}

if(in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)) {
    ?>
	<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
	        <?php 
	        // ARMAZENA DADOS DA ABA PAI
	        echo "<input type='hidden' value='{$abaMestre}' id='abaMestre' name='abaMestre' />";
	        echo "<input type='hidden' value='{$acao}' id='acao' name='acao' />";
	        ?>
	    	<input type="hidden" name="angtip" value="<?php echo $tipoArquivo; ?>">
	    	<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
	    		<tr>
	    			<td class="SubtituloDireita" style="width: 33%">Arquivo:</td>
	    			<td>
	    				<input type="file" name="arquivo" id="arquivo" />
	    				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
	    			</td>
	    		</tr>
	    		<tr>
	    			<td class="SubtituloDireita" style="width: 33%">Descrição:</td>
	    			<td>
	    				<?php echo campo_textarea("descricao","N","S","", 60, 3, 255); ?>
					</td>
	    		</tr>
	    		<tr>
	    			<td class="SubtituloDireita" style="width: 33%">&nbsp;</td>
	    			<td class="SubtituloEsquerda">
	    				<input type="button" name="botao" class="botao" value="Enviar" onclick="enviaArquivos();"/>
	    			</td>
	    		</tr>
	
	    	</table>
	</form>
    <?

	$btnExcluir = "<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirAnexo(\''||arq.arqid||'\');\">";

}else{
    // ARMAZENA DADOS DA ABA PAI
    echo "<input type='hidden' value='{$abaMestre}' id='abaMestre' name='abaMestre' />";
    echo "<input type='hidden' value='{$acao}' id='acao' name='acao' />";
    
}// FECHA if(in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)) {

$sql = "SELECT 
            '<center><a href=\"projovemurbano.php?modulo=principal/bancoDeQuestoes&acao=".$acao."&download=S&arqid='|| arq.arqid ||'\"><img src=../imagens/anexo.gif></a>&nbsp; $btnExcluir</center>' as acoes,
            '<a href=\"projovemurbano.php?modulo=principal/bancoDeQuestoes&acao=".$acao."&download=S&arqid='|| arq.arqid ||'\">' || arq.arqnome || '.' || arq.arqextensao || '</a>' as arquivo,
            arq.arqdescricao,
            arq.arqtamanho,
            to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora as arqdata,
            usu.usunome
        FROM 
        	projovemurbano.anexogeral ang
        INNER JOIN public.arquivo    arq on ang.arqid  = arq.arqid
        INNER JOIN seguranca.usuario usu on arq.usucpf = usu.usucpf
        WHERE 
        	ang.angtip = '".$tipoArquivo."'
			AND ppuid = {$_SESSION['projovemurbano']['ppuid']}";
//echo'<pre>';echo($sql);

$cabecalho = array( "Ações","Arquivo" , "Descrição", "Tamanho (bytes)", "Data inclusão", "Responsável");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
?>
