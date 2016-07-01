<?php
verificaAnoRef();
verificaMuncod();

// Aumenta limite de memoria para importar o arquivo
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// Arquivos necessarios
include_once APPRAIZ . 'includes/cabecalho.inc';
include_once APPPISO . 'classes/FolhaPagamento.class.inc';
include_once APPPISO . 'classes/FolhaPagamentoProfissionais.class.inc';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPPISO . 'classes/excel_reader2.class.inc';

// Montar abas
print '<br/>';
$abacod_tela = '57365';
$arMnuid = array(ABA_FP_DUPLICAR_MES, ABA_FP_LISTAR_PROFIS, ABA_FP_FORM_PROFISSIONAL);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

monta_titulo('Importar Arquivo', "Folha de Pagamento");

$obFolhaPagamento = new FolhaPagamento();
$obFolhaProfissionais = new FolhaPagamentoProfissionais();

$arDados = array('muncod'           => $_SESSION['piso']['muncod'],
                 'flpanoreferencia' => $_REQUEST['anoref'],
                 'flpmesreferencia' => $_REQUEST['mesref']);

$obFolhaPagamento->gerarFolhaPagamento($arDados);

if( $_FILES['arquivo']['name'] ){
    $obFolhaProfissionais->importarArquivoExcel();
}

montaCabecalho();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

function download_file(arqid){
    window.open('download_file.php?arqid='+arqid, 'download', 'height=500,width=400,scrollbars=yes,top=50,left=200');
}

/**
 * Comment
 */
function view_log(arqid) 
{    
    //console.info(arqid);
    document.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/importarArquivo&acao=A&arqid=' + arqid + '&mostralog=true&anoref=' + <?php echo $_REQUEST['anoref'] ?> + '&mesref=' + <?php echo $_REQUEST['mesref'] ?>;
}

/**
 * Submete o formulário
 * @name enviaArquivos
 * @return void
 */
function enviaArquivos( arquivo ){
    var formulario = jQuery('#formulario');

   extensoes_permitidas = new Array(".xls");
   meuerro = "";
   if (!arquivo) {
      //Se não tenho arquivo, é porque não se selecionou um arquivo no formulário.
        meuerro = "Por favor, selecionar o arquivo que deseja anexar para realizar essa ação.";
   }else{
      //recupero a extensão deste nome de arquivo
      extensao = (arquivo.substring(arquivo.lastIndexOf("."))).toLowerCase();
      //comprovo se a extensão está entre as permitidas
      permitida = false;
      for (var i = 0; i < extensoes_permitidas.length; i++) {
         if (extensoes_permitidas[i] == extensao) {
         permitida = true;
         break;
         }
      }
      if (!permitida) {
         meuerro = "Somente arquivos com extensão .XLS poderão ser anexados.";
        }else{
            jQuery('#requisicao').val('salvar');
            formulario.submit();
            return 1;
        }
   }
   //se estou aqui é porque não se pode submeter
   alert (meuerro);
   return 0;
}
jQuery.noConflict();

jQuery(document).ready(function(){
    jQuery('.voltar').click(function(){
        document.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/listaFolhaPagamento&acao=A';
    });

});
</script>
<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
        <tr>
            <td colspan="2" class="subtitulocentro">
                <a href="javascript:alert('Manual não disponível')">Manual para carga</a>
                &nbsp;/&nbsp;
                <a href="arquivos/piso_salarial_planilha_de_importacao_de_dados.xls.zip">Planilha padrão</a>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" style="width: 33%">Arquivo:</td>
            <td>
                <input type="hidden" name="anoref" value="<?php echo $_REQUEST['anoref'] ?>">
                <input type="hidden" name="mesref" value="<?php echo $_REQUEST['mesref'] ?>">
                <input type="file" name="arquivo" id="arquivo" />
            </td>
        </tr>
        <tr bgcolor="#cccccc">
           <td></td>
           <td>
            <input type="button" name="botao" value="Enviar" onclick="enviaArquivos(this.form.arquivo.value);"/>
            <input type="button" value="Cancelar" class="voltar" />
            <input type="button" value="Voltar" class="voltar" />
           </td>
        </tr>
    </table>
</form>
<?php
if($_REQUEST['mostralog'] == 'true'){
    $obFolhaProfissionais->listaLogArquivo( $_REQUEST['arqid'] );
}

//if($_REQUEST['mostralog'] == 'true'){
//    $obFolhaPagamento->listaArquivosImportados();
//}
?>
<br/><br/>
<center>
<div style="width:300px">
<?php
    $obFolhaPagamento->listaArquivosImportados();
?>
</div>
</center>