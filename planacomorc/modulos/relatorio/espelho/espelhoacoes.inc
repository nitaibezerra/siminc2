<?php
/**
 * Tela inicial do módulo planacomorc com o menu de opções disponíveis no sistema.
 * $Id: inicio.inc 74442 2014-01-29 19:36:36Z werteralmeida $
 */
/**
 *
 */
include_once '_funcoesacoes.php';

if ($_REQUEST['download'] == 'S') {
	$file = new FilesSimec ();
	$arqid = $_REQUEST ['arqid'];
	ob_clean ();
	$arquivo = $file->getDownloadArquivo ( $arqid );
	exit ();
}

// -- Setando o novo exercicio escolhido via seletor no cabeçalho da página
if (isset($_REQUEST['exercicio'])) {
    $_SESSION['exercicio'] = $_REQUEST['exercicio'];
}
include APPRAIZ . "includes/cabecalho.inc";
?>
<style>
    .divGestao{
        width: 330px;

        float: left;
        margin-top: 20px;
        margin-left: 30px;
        -webkit-border-radius: 20px;
        -moz-border-radius: 20px;
        border-radius: 20px;
        clear: right;
        height: 100%;
        padding-bottom: 10px;
    }

    .tituloCaixa{
        float: left;
        margin-left: 10px;
        margin-top: 10px;
        color: #FFF;
        font-size: 18px;
        width: 90%;
        border-bottom: 1px solid #FFF;
        padding-bottom: 3px;
        text-align: left;
    }
    .subTituloCaixa{
        float: left;
        margin-left: 10px;
        margin-top: 10px;
        color: #FFF;
        font-size: 12px;
        width: 90%;
        border-bottom: 1px solid #FFF;
        padding-bottom: 3px;
        text-align: left;
    }
    .btnNormal{
        background-color: #FFF;
        -webkit-border-radius: 5px;
        -moz-border-radius: 5px;
        border-radius: 5px;
        float: left;
        width: 140px;
        color: #002A6C;
        font-weight: bold;
        text-align: center;
        margin-top: 10px;
        margin-left: 10px;
        cursor: pointer;
        padding:5px;
        color: #002A6C;
        height: 45px;
    }
    .btnNormal:hover{
        background-color: #C0C0C0;
    }
    #divBasica{
        background-color: yellowgreen;
    }
    #divSubacoes{
        background-color:#00CED1 ;
    }
    #divTecnologica{
        background-color:#FF6347 ;
    }
    #divAdministrar{
        background-color:#EEB422 ;
    }
    #divSuperior{
        background-color: darksalmon ;
    }
    #divPrevRecOrc{
        background-color: #CC6666  ;
    }
    #divTabelaApoio{
        background-color: #18bc9c  ;
    }
</style>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

$(document).ready(function() {
    $('.btnOn').click(function() {
        var uri = $(this).attr('data-request');
        if (!uri) {
            alert('Botão sem url (data-request) definida.');
            return;
        }
        location.href = uri;
    });
    $('.btnNovaJanela').click(function() {
        var uri = $(this).attr('data-request');
        if (!uri) {
            alert('Botão sem url (data-request) definida.');
            return;
        }
        window.open(uri);
    });
});

    $(document).ready(function()
    {
        /* Ações */
        $('#btncadastro').click(function() {
            location.href = 'planacomorc.php?modulo=relatorio/espelho/cadastro&acao=A';
        });
        $('#btnAcoesLista').click(function() {
            location.href = 'planacomorc.php?modulo=relatorio/espelho/listagem&acao=A';
        });
        
   });
</script>
<div class="col-md-12">
    <ol class="breadcrumb">
    	<li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
    	<li class="">Ações <?= $_SESSION['exercicio']; ?></li>
    	<li class="active">Espelho das Ações</li>
    </ol>
</div>
<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center" >
        <div id="divAdministrar" class="divGestao">
                <span class="tituloCaixa">Administrar</span> <br><br><br>
                <div class="btnNormal" id="btncadastro">
                    <img src="css/images/chart-up.png" /> Cadastrar
                </div>
                <div class="btnNormal" id="btnAcoesLista">
                    <img src="css/images/blue-document-list.png" /> Listar
                </div>
             </div>
            <div id="divBasica" class="divGestao">
                <span class="tituloCaixa">Educação Básica</span> <br><br><br>
                <?php 
                $sql="SELECT
                            arq.arqid,
                            con.angdsc,
                            arq.arqtamanho,
                            angtip,
                            to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora as arqdata,
                            usu.usunome		
                     FROM
                            planacomorc.anexogeral con
                     INNER JOIN 
                            public.arquivo arq on con.arqid = arq.arqid
                     INNER JOIN 
                            seguranca.usuario usu on usu.usucpf = arq.usucpf
                     WHERE angtip = 'BA' 
                     AND con.angtipoanexo = 'E' 
                     AND con.angano = '".$_SESSION['exercicio']."'";
                //ver($sql);
                $dados = $db->carregar($sql);
                if($dados){
                    foreach ( $dados as $arquivos ) {
                                 echo "<div id=\"".$arquivos[angdsc]."\" 
                                       class=\"btnNormal btnNovaJanela\" style=\"height: auto;\" data-target=\"_new\" data-request=\"planacomorc.php?modulo=relatorio/espelho/listagem&acao=A&download=S&arqid=".$arquivos['arqid']."\">
                                       <img src=\"css/images/book-bookmark.png\" />". 
                    $arquivos[angdsc]."</div>";
                   }
               }else{
                   echo "Sem registros";
               }
                ?>
              
                    </div>
            <div class="divGestao" id="divSubacoes">
                <center>
                    <span class="tituloCaixa">Programa de Gestão</span><br><br><br>
                </center>
                <?php 
                    $sql="SELECT con.angdsc,
                                 arq.arqtamanho,
                                 angtip,
                                 to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora as arqdata,
                                 usu.usunome		
                          FROM planacomorc.anexogeral con
                          INNER JOIN public.arquivo arq on con.arqid = arq.arqid
                          INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
                          WHERE angtip = 'GE' AND con.angtipoanexo = 'E' AND con.angano = '".$_SESSION['exercicio']."'";
                    //ver($sql);
                    $dados = $db->carregar($sql);
                    if($dados){
                        foreach ( $dados as $arquivos) {
                            echo              "<div id=\"".$arquivos[angdsc]."\" 
                                                    class=\"btnNormal btnNovaJanela\" style=\"height: auto;\" data-request=\"planacomorc.php?modulo=relatorio/espelho/listagem&acao=A&download=S&arqid=".$arquivos['arqid']."\">
                                                    <img src=\"css/images/book-bookmark.png\" />". 
                            $arquivos[angdsc]."</div>";
                        }
                    }else{
                        echo "Sem registros.";
                    }
                ?>
            </div>

            <div id="divTecnologica" class="divGestao">
                <center>
                    <span class="tituloCaixa" >Programa de Educação Tecnológica</span> <br><br><br>
                </center>
             <?php 
                $sql="SELECT arq.arqid,
                             con.angdsc,
                             arq.arqtamanho,
                             angtip,
                             to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora as arqdata,
                             usu.usunome		
                      FROM planacomorc.anexogeral con
                      INNER JOIN public.arquivo arq on con.arqid = arq.arqid
                      INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
                      WHERE angtip = 'TE' AND con.angtipoanexo = 'E' AND con.angano = '".$_SESSION['exercicio']."'";
                //ver($sql);
                $dados = $db->carregar($sql);
                if($dados){
                    foreach ( $dados as $arquivos) {
                        echo                 "<div id=\"".$arquivos['angdsc']."\" 
                                                class=\"btnNormal btnNovaJanela\" style=\"height: auto;\" data-request=\"planacomorc.php?modulo=relatorio/espelho/listagem&acao=A&download=S&arqid=".$arquivos['arqid']."\">
                                                <img src=\"css/images/book-bookmark.png\" />". 
                        $arquivos['angdsc']." </div>";
                    }
                }else{
                    echo "Sem registros.";
                }
             ?>
               
            </div>
            <div id="divSuperior" class="divGestao">
                <span class="tituloCaixa" >Programa Educação Superior</span> <br><br><br>
               <?php 
                $sql="SELECT arq.arqid,
                             con.angdsc,
                             arq.arqtamanho,
                             angtip,
                             to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora as arqdata,
                             usu.usunome		
                      FROM planacomorc.anexogeral con
                      INNER JOIN public.arquivo arq on con.arqid = arq.arqid
                      INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
		      WHERE angtip = 'SU' AND con.angtipoanexo = 'E' AND con.angano = '".$_SESSION['exercicio']."'";
                //ver($sql);
                $dados = $db->carregar($sql);
                if($dados){
                    foreach ( $dados as $arquivos) {
                            echo      "<div id=\"".$arquivos[angdsc]."\" 
                                            class=\"btnNormal btnNovaJanela\" style=\"height: auto;\" data-request=\"planacomorc.php?modulo=relatorio/espelho/listagem&acao=A&download=S&arqid=".$arquivos['arqid']."\">
                                            <img src=\"css/images/book-bookmark.png\" />". 
                    $arquivos[angdsc]."</div>";
                    }
               }else{
                    echo "Sem registros.";
                }
                ?>
            </div>
           
        </td>
    </tr>
</tr>
</table>
