<?php

// Inclui classes necessarias para utilizacao do modulo
include_once APPRAIZ."sca/classes/Visitante.class.inc";

$oVisitante = new Visitante();

if($_REQUEST['req'] == 'ACvttdoc'){
    ob_clean();
    header('Content-Type: text/html; charset=iso-8859-1');

    $q = strtolower($_GET["q"]);
    if( $q === '') return;

    $oVisitante->listarAutorizacoesPorFiltro($q, '', '');
    die;
} //vstdatentrada
else if($_REQUEST['req'] == 'ACvttnome'){
    ob_clean();
    header('Content-Type: text/html; charset=iso-8859-1');

    $q = strtolower($_GET["q"]);
    if ($q === '') return;

    $oVisitante->listarAutorizacoesPorFiltro('', $q, '');
    die;
}
else if($_REQUEST['req'] == 'ACautdatinicio'){
    ob_clean();
    header('Content-Type: text/html; charset=iso-8859-1');

    $q = strtolower($_GET["q"]);
    if ($q === '') return;

    $oVisitante->listarAutorizacoesPorFiltro('', '', '',$q );
    die;
}
else if($_REQUEST['req'] == 'ACautdatfinal'){
    ob_clean();
    header('Content-Type: text/html; charset=iso-8859-1');

    $q = strtolower($_GET["q"]);
    if ($q === '') return;

    $oVisitante->listarAutorizacoesPorFiltro('', '', '', '', $q);
    die;
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

// seta o $abacod_tela, que esta retornando nullo.
if(is_null($abacod_tela)){
	$abacod_tela = 57300;
}

$db->cria_aba($abacod_tela,$url,'');
monta_titulo( 'Consulta de Autorizações de Acesso Fora do Horário', '&nbsp;' );

?>
<script language="javascript" type="text/javascript" src="./js/sca.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
 <script src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
<script language="javascript" type="text/javascript">

    jQuery.noConflict();

    //função que dispara a pesquisa no banco
    function pesquisar(acao, status){

        if(acao == 'todos'){
            $('vttdoc').setValue('');
            $('vttnome').setValue('');
           // $('vstdatentrada').setValue('');//no_unidade_org
            //$('no_unidade_org').setValue('');
        }
        var url = 'sca.php?modulo=principal/visitante/registrarAcessoForaHorario&acao=A&pesquisar='+acao+'&status='+status;
        var formulario = document.getElementById("formulario");
        formulario.action = url;
        formulario.submit();
    }

    // preenche a div com um sinal de carregando
    carregando = function(){
        
        jQuery('#requestAjax').html('<div style="background-color: rgb(255, 255, 255); color: rgb(0, 0, 51); margin:auto auto auto auto;padding-left:30px; border: 2px solid rgb(204, 204, 204); width: 300px; font-size: 12px; z-index: 0;" id="aguarde"><br><img border="0" align="middle" src="../imagens/wait.gif"> &nbsp;&nbsp; Aguarde! Carregando Dados...<br><br></div>');
        
    }

    jQuery(document).ready(function() {

        var requestXHR = null;

        jQuery('#vttdoc').keyup(function(){
            

            
            if( this.value != '' )
            {
                jQuery('div#requestAjax').empty();
                if(requestXHR) requestXHR.abort();
                requestXHR = jQuery.ajax({
                  url: "sca.php?modulo=principal/visitante/registrarAcessoForaHorario&acao=A&req=ACvttdoc&q=" + this.value,
                  dataType: 'html',
                  beforeSend: carregando,
                  success: function(data){
                      jQuery('#requestAjax').html(data);
                  }
                });
            }
        });

        jQuery('#vttnome').keyup(function(){
            // Retira caracteres invalidos
            this.value = retirararCaracteresEspeciaisTexto( this.value );
            
            if( this.value != '' )
            {
                if(requestXHR) requestXHR.abort();
                requestXHR = jQuery.ajax({
                  url: "sca.php?modulo=principal/visitante/registrarAcessoForaHorario&acao=A&req=ACvttnome&q=" + this.value,
                  dataType: 'html',
                  beforeSend: carregando,
                  success: function(data){
                    jQuery('#requestAjax').html(data);
                  }
                });
            }
        });
    });
</script>
<form name="formulario" id="formulario" method="post">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td class="SubTituloDireita">Número do Documento:</td>
            <td colspan="3"><?=campo_texto('vttdoc','','','',30,20,'','','left','',0,'id="vttdoc"','this.value=formatarNumeroDocumento(this.value,null);',$_REQUEST['vttdoc'],'this.value=formatarNumeroDocumento(this.value,null);');?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome:</td>
            <td colspan="3"><?=campo_texto('vttnome','','','',60,100,'','','left','',0,'id="vttnome"','',$_REQUEST['vttnome']);?></td>
        </tr>
        <tr>
        <tr>
            <td class="SubTituloDireita">Data Inicial:</td>
                <td>
                    <?php if( !empty( $_REQUEST['autdatinicio'] ) ): ?>
                        <?=campo_data2('autdatinicio',false,true,'','S','Data Inicial','', ajusta_data( $_REQUEST['autdatinicio'] ) );?>
                    <?php else: ?>
                        <?=campo_data2('autdatinicio',false,true,'','N','Data Inicial','', NULL );?>
                    <?php endif; ?>
                </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data Final:</td>
                <td>
                    <?php if( !empty( $_REQUEST['autdatfinal'] ) ): ?>
                        <?=campo_data2('autdatfinal',false,true,'','S','Data Final','', ajusta_data( $_REQUEST['autdatfinal'] ) ); ?>
                    <?php else: ?>
                        <?=campo_data2('autdatfinal',false,true,'','N','Data Final','', NULL ); ?>
                    <?php endif; ?>
                </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Unidade Organizacional:</td>
            <td colspan="3"><?=campo_texto('no_unidade_org','','','',60,100,'','','left','',0,'id="no_unidade_org"','',$_REQUEST['no_unidade_org']);?></td>
        </tr>
        
        <tr>
            <td bgcolor="#CCCCCC" colspan="4" align="center">
                <input type="button" name="visualizar" value="Visualizar" style="cursor:pointer;" onclick="pesquisar('filtrar','');">
                <input type="button" name="vertodos" value="Ver Todos" style="cursor:pointer;" onclick="pesquisar('todos','');">
            </td>
        </tr>
        <tr>
            <td colspan="4">Legenda:</td>
        </tr>
        <tr>
            <td style="width:25%"><a href="javascript:pesquisar('filtrar','1')"><img src='../imagens/p_azul.gif' style="margin-right:10px">Autorização Permanente</a></td>
            <td style="width:25%"><a href="javascript:pesquisar('filtrar','2')"><img src='../imagens/p_verde.gif' style="margin-right:10px">Autorização Vigente</a></td>
            <td style="width:25%"><a href="javascript:pesquisar('filtrar','3')"><img src='../imagens/p_amarelo.gif' style="margin-right:10px">Autorização Futura</a></td>
            <td style="width:25%"><a href="javascript:pesquisar('filtrar','4')"><img src='../imagens/p_vermelho.gif' style="margin-right:10px">Autorização Vencida</a></td>
        </tr>
    </table>
</form>
<script>
    Event.observe(window, 'load', function() {
        $("vttdoc").focus();
      });

    ajustarAbas();
</script>

<br />

<div id="requestAjax">
<?php
//verifica se é para disparar a consulta no banco
if(!empty($_REQUEST['pesquisar'])){
    if($_REQUEST['pesquisar'] == 'filtrar'){
        $oVisitante->listarAutorizacoesPorFiltro(
            $_REQUEST['vttdoc'],
            $_REQUEST['vttnome'],
            $_REQUEST['status'],
            $_REQUEST[ 'autdatinicio' ],
            $_REQUEST[ 'autdatfinal' ],
            $_REQUEST[ 'no_unidade_org' ]
        ); // no_unidade_org
        
    }elseif ($_REQUEST['pesquisar'] =='todos'){
        $oVisitante->listarAutorizacoesPorFiltro('', '', '', '', '');
    }
}else{
    //$oVisitante->listarAutorizacoesPorFiltro('', '', '', '', '');
}
?>
</div>

<script type="text/javascript">
    (function(){$$('.listagem td.title').invoke('removeAttribute','onclick');})();
</script>
