<?php
if(isset($_SESSION['obras2']['supid'])){ unset($_SESSION['obras2']['supid']); }

$obrid = !empty($_SESSION['obras2']['obrid']) ? $_SESSION['obras2']['obrid'] : $_GET['obrid'];
$emiid = $_GET['emiid'];

if(empty($obrid) && empty($emiid)){
    verificaSessao('obra');    
}

if($emiid){
    $evoMi = new EvolucaoMi();
    $arrDadosEvolucaoMi = $evoMi->getDadosEvolucaoMi($emiid);
    $obrid = $arrDadosEvolucaoMi['obrid'];
}

$obr = new Obras($obrid);

$_SESSION['obras2']['empid'] = $obr->empid;
$_SESSION['obras2']['obrid'] = $obr->obrid;

if(!empty($_GET['emiid']) && empty($_SESSION['obras2']['emiid'])){
    $_SESSION['obras2']['emiid'] = $_GET['emiid'];
}

// empreendimento || obra || orgao
verificaSessao('obra');

if ($_REQUEST['ajaxFotosGaleria'] && ($_REQUEST['arrFotosSupervisao'] || $_REQUEST['arrFotosGaleria']) ) {
    if (atualizarFotosEvolucaoMI($_SESSION['obras2']['emiid'])) { echo "true"; } else { echo "false"; }
    exit();
}

switch ($_REQUEST['req']) {
    //Requisição, SALVAR
    case 'cadastroNovaEvoFotos':
        $evoMi    = new EvolucaoMi();
        $response = $evoMi->gravarDadosEvoluçãoMiParaFotos();
        if(!$arr_return['erro']){
            echo $response['emiid'];
        }else{
            echo 'false';
        }
        die();
        break;
    case 'cadastro':
        $evoMi    = new EvolucaoMi();
        $response = $evoMi->gravarDadosEvoluçãoMi('salvar');

        if(!is_array($response)){
            die("<script>
                    alert('".addslashes($response)."'); 
                    window.history.back();
                 </script>");
        }else{
//            $dadosHtml = serialize( array('emiid'=>$response['emiid']) );
            $dh = array('emiid'=>$response['emiid']);
            $dadosHtml = serialize( $dh );
            $strVeri = urlencode( $dadosHtml );
            die("<script>

                    function wf_alterarEstado( aedid, docid, esdid, acao )
                    {
                            var url = 'http://".$_SERVER['SERVER_NAME']."/geral/workflow/alterar_estado.php' +
                                      '?aedid=' + aedid +
                                      '&docid=' + docid +
                                      '&esdid=' + esdid +
                                      '&verificacao=".$strVeri."';

                            var janela = window.open(
                                    url,
                                    'alterarEstado',
                                    'width=550,height=500,scrollbars=no,scrolling=no,resizebled=no'
                            );
                            janela.focus();

                            myVar = setTimeout(function(janela){janela.close();},1500);
                    }

                    alert('".$response['msg']."'); 

                    var aedid = ".AEDID_MI_ENVIAR_VALIDACAO." ;
                    var docid = ".$response['docid'].";
                    var esdid = ".ESDID_MI_VALIDACAO."; //Validação
                    var acao  = 'Enviar para validação';

                    wf_alterarEstado( aedid, docid, esdid, acao );

                    window.location = '?modulo=principal/listaEvolucaoMi&acao=A';

                 </script>");
        }
        break;
    //Requisição, Excluir
    case 'excluir':
        $emiid    = $_GET['emiid'];
        $evoMi    = new EvolucaoMi();
        $estado = $evoMi->getEstadoWfEvolucaoMI($emiid);
        if($estado['esdid'] !== ESDID_MI_CONCLUIDO){
            $response = $evoMi->excluirEvolucaoMi($emiid);
            if($response !== true){
                die("<script>
                        alert('".$response."'); 
                        window.history.back();
                     </script>");
            }else{
                die("<script>
                        alert('Evolução MI excluida com sucesso!'); 
                        window.location = '?modulo=principal/listaEvolucaoMi&acao=A';
                     </script>");
            }
        }else{
            die("<script>
                    alert('Não é possível excluir a Evolução MI, pois a mesma já se econtra concluída.'); 
                    window.history.back();
                 </script>");
        }
        break;
    //Requisição, Edição
    case 'edicao':
        $emiid = $_GET['emiid'];
        $evoMi    = new EvolucaoMi();
        $estado = $evoMi->getEstadoWfEvolucaoMI($emiid);
        $docid  = $evoMi->getDocidEmiid($emiid);
        if($estado['esdid'] !== ESDID_MI_CONCLUIDO && $estado['esdid'] !== ESDID_MI_VALIDACAO){
            $response = $evoMi->gravarDadosEvoluçãoMi('atualizar');        
            $erro     = $response['erro'];
            $docid    = $response['docid'];
            if($erro == true){
                die("<script>
                        alert('".$response['msg']."'); 
                        window.history.back();
                     </script>");
            }else{
                
                if($estado['esdid'] == ESDID_MI_CADASTRAMENTO){
                    $dh = array('emiid'=>$response['emiid']);
                    $dadosHtml = serialize( $dh );
                    $strVeri = urlencode( $dadosHtml );
                    die("<script>

                            function wf_alterarEstado( aedid, docid, esdid, acao )
                            {
                                    var url = 'http://".$_SERVER['SERVER_NAME']."/geral/workflow/alterar_estado.php' +
                                              '?aedid=' + aedid +
                                              '&docid=' + docid +
                                              '&esdid=' + esdid +
                                              '&verificacao=".$strVeri."';

                                    var janela = window.open(
                                            url,
                                            'alterarEstado',
                                            'width=550,height=500,scrollbars=no,scrolling=no,resizebled=no'
                                    );
                                    janela.focus();

                                    myVar = setTimeout(function(janela){janela.close();},1500);
                            }

                            alert('".$response['msg']."'); 

                            var aedid = ".AEDID_MI_ENVIAR_VALIDACAO." ;
                            var docid = ".$response['docid'].";
                            var esdid = ".ESDID_MI_VALIDACAO."; //Validação
                            var acao  = 'Enviar para validação';

                            wf_alterarEstado( aedid, docid, esdid, acao );

                            window.location = '?modulo=principal/listaEvolucaoMi&acao=A';

                         </script>");
                }else{
                    $dh = array('emiid'=>$emiid);
                    $dadosHtml = serialize( $dh );
                    $strVeri = urlencode( $dadosHtml );

                    die("<script>
                            function wf_alterarEstado( aedid, docid, esdid, acao )
                            {
                                    var url = 'http://".$_SERVER['SERVER_NAME']."/geral/workflow/alterar_estado.php' +
                                              '?aedid=' + aedid +
                                              '&docid=' + docid +
                                              '&esdid=' + esdid +
                                              '&verificacao=".$strVeri."';

                                    var janela = window.open(
                                            url,
                                            'alterarEstado',
                                            'width=550,height=500,scrollbars=no,scrolling=no,resizebled=no'
                                    );
                                    janela.focus();
                                    myVar = setTimeout(function(janela){janela.close();},1500);
                            }

                            alert('".$response['msg']."'); 

                            var aedid = ".AEDID_MI_ENVIAR_VALIDACAO_POS_CORRECAO."; //Enviar para validação, vindo da correção
                            var docid = ".$docid.";
                            var esdid = ".ESDID_MI_VALIDACAO."; //Validação
                            var acao  = 'Enviar para validação';

                            wf_alterarEstado( aedid, docid, esdid, acao );

                            window.location = '?modulo=principal/listaEvolucaoMi&acao=A';
                         </script>");
                }
                
            }
        }else{
            die("<script>
                    alert('Não é possível editar a Evolução MI, verifique o estado que a mesma se encontra.'); 
                    window.history.back();
                 </script>");
        }
        break;
    //Requisição, Validação
    case 'validacao':
        $emiid    = $_GET['emiid'];
        $evoMi    = new EvolucaoMi();
        $estado = $evoMi->getEstadoWfEvolucaoMI($emiid);
        if($estado['esdid'] == ESDID_MI_VALIDACAO){
            $response = $evoMi->gravarDadosEvoluçãoMi('validar');        
            $erro     = $response['erro'];        
            if($erro == true){
                die("<script>
                        alert('".$response['msg']."'); 
                        window.history.back();
                     </script>");
            }else{
                die("<script>
                        alert('".$response['msg']."'); 
                        window.location = '?modulo=principal/listaEvolucaoMi&acao=A';
                     </script>");
            }
        }else{
            die("<script>
                    alert('Não é possível validar a Evolução MI, pois o estado da mesma deve ser \'Validação\'.'); 
                    window.history.back();
                 </script>");
        }
        break;
}

include APPRAIZ . "includes/cabecalho.inc";

echo '<script language="JavaScript" src="../includes/funcoes.js"></script>
      <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
      <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

$db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros);
echo cabecalhoObra($obrid);
echo alertaObraMi($obrid);

switch ($_GET['acao']) {
    // Ação padrão, Cadastro
    case 'A':
        $titulo_pg        = 'Cadastro de Evolução MI';
        $req              = 'cadastro';
        $emiid            = 0;
        $btn_salvar_form  = '<input type="button" value="Salvar e enviar para Validação" id="btn_salvar_evolucao" style="cursor:pointer;"/> &nbsp; ';
        $btn_cancela_form = '<input type="button" value="Voltar" id="btn_voltar" style="cursor:pointer;"/>';
        break;
    // Ação, Edição
    case 'E':
        $titulo_pg        = 'Edição da Evolução MI';
        $req              = 'edicao';
        $emiid            = $_REQUEST['emiid'];
        $btn_salvar_form  = '<input type="button" value="Salvar e enviar para Validação" id="btn_salvar_evolucao" style="cursor:pointer;"/> &nbsp; ';
        $btn_cancela_form = '<input type="button" value="Cancelar Edição" id="btn_voltar" style="cursor:pointer;"/>';
        break;
    // Ação, Validação
    case 'V':
        $titulo_pg        = 'Validação de Evolução MI';    
        $req              = 'validacao';
        $emiid            = $_REQUEST['emiid'];
        $btn_salvar_form  = '<input type="button" value="Salvar " id="btn_salvar_evolucao" style="cursor:pointer;"/> &nbsp; ';
        $btn_cancela_form = '<input type="button" value="Cancelar Validação" id="btn_voltar" style="cursor:pointer;"/>';
        break;
    // Ação, Edita Validação
    case 'EV':
        $titulo_pg        = 'Edita a Validação de Evolução MI';    
        $req              = 'editaValidacao';
        $emiid            = $_REQUEST['emiid'];
        $btn_salvar_form  = '<input type="button" value="Salvar " id="btn_salvar_evolucao" style="cursor:pointer;"/> &nbsp; ';
        $btn_cancela_form = '<input type="button" value="Cancelar Validação" id="btn_voltar" style="cursor:pointer;"/>';
        break;
}

$_SESSION['obras2']['emiid'] = $emiid;

$bloqueiaForm      = 'false';
$avisoBloqueioForm = '';

$arrDadosEvolucaoMi = array();

if($_GET['acao'] !== 'A'){
    $evoMi = new EvolucaoMi();
    $arrDadosEvolucaoMi = $evoMi->getDadosEvolucaoMi($emiid);
    
    //Regras de manipulação dos dados
    switch ($arrDadosEvolucaoMi['esdid']) {
        case ESDID_MI_CADASTRAMENTO:
            if($_GET['acao'] == 'V'){
                $bloqueiaForm = 'true';
                $avisoBloqueioForm = 'A Evolução não poderá ser validada pois ainda se encontra "Em Cadastramento".';
            }
            break;
        case ESDID_MI_VALIDACAO:
            if($_GET['acao'] == 'E'){
                $bloqueiaForm = 'true';
                $avisoBloqueioForm = 'A Evolução não poderá ser editada pois se encontra em "Validação".\n Para editá-la envie para o estado "Em Cadastramento" ou "Correção".';
            }
            break;
        case ESDID_MI_CONCLUIDO:
                $bloqueiaForm = 'true';
                $avisoBloqueioForm = 'A Evolução não poderá sofre nenhuma alteração pois se encontra "Concluída".';
            break;
        case ESDID_MI_CORRECAO:
            if($_GET['acao'] == 'V'){
                $bloqueiaForm = 'true';
                $avisoBloqueioForm = 'A Evolução não poderá ser validada pois ainda se encontra em "Correção".';
            }
            break;

        default:
            break;
    }
}

$arPerfilEmpresaMI = array(PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL,PFLCOD_SUPER_USUARIO);
if(($_GET['acao'] == 'A' || $_GET['acao'] == 'E') && !possuiPerfil($arPerfilEmpresaMI)){ 
     $bloqueiaForm      = 'true';
     $avisoBloqueioForm = 'Você não possui permissão para Salvar/Editar os dados.';
}

$arPerfilGestaoMI = array(PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE,PFLCOD_SUPER_USUARIO);
if( ($_GET['acao'] == 'V' || $_GET['acao'] == 'EV') && !possuiPerfil($arPerfilGestaoMI) ){
     $bloqueiaForm      = 'true';
     $avisoBloqueioForm = 'Você não possui permissão para Validar os dados.';
}
//demanda 311298 -- O perfil call center não pode alterar/inserir nenhuma informação.
if(possui_perfil(PFLCOD_CALL_CENTER)){
    $bloqueiaForm      = 'true';
    $avisoBloqueioForm = 'Você não possui permissão para Editar dados.';
}

if($emiid && isset($arrDadosEvolucaoMi['esdid']) && $arrDadosEvolucaoMi['esdid'] != ESDID_MI_CONCLUIDO){
    $habilitadoFoto = true;    
}else{
    $habilitadoFoto = false;
}

monta_titulo($titulo_pg, '');

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<!--<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>-->
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>

<script type="text/javascript">
    
function calculosEvolucao() {
    
    var objTbodyCalc;
    var objLinhaTr;
    var objLinhaTotais;
    var objCampoEditado;
    
    var vlrObra;
    var vlrItemObra;
    var percentItemObra;
    
    var qtdItemObraExec;
    var vlrItemObraExec;
    var percentItemObraExec;
    
    var qtdItemObraValid;
    var vlrItemObraValid;
    var percentItemObraValid;
    
    var valorItemObraExecSobreObra;
    var percentItemObraExecSobreObra;
    
    var tipoDado;  //D: Itens cronograma //F: Itens Serviços externos //DV: Itens cronograma - Validação  //FV: Itens Serviços externos - Validação
    var tipoCampo; // % // vl // qtd
    /** OK **/
    this.keyupAction = function(obj, tipoDado, tipoCampo){
        
        this.resetAtributos();
        
        this.value = mascaraglobal('###.###.###.###,##',obj.value);
        
        this.tipoDado  = tipoDado;
        this.tipoCampo = tipoCampo;
        
        this.objCampoEditado  = $(obj);
        this.objLinhaTr       = $(obj).parents('tr:eq(0)');
        
        var id_linha          = this.objLinhaTr.attr('id');
        this.objLinhaTr       = $('#'+id_linha);
        
        this.vlrItemObra = this.strVlToFloat(this.objLinhaTr.find('td:eq(1)').html());
        
        /** OK **/
        if(this.tipoDado == 'D'){
            this.objLinhaTotais = $('#linha_totais_d');
            this.vlrObra = this.strVlToFloat(this.objLinhaTotais.find('th:eq(1)').html());
            
            switch(this.tipoCampo){
                case '%':
                    if(this.objCampoEditado.val().trim() === ''){
                        this.objCampoEditado.val('0,00');
                    }
                    this.percentItemObraExec = this.validaMaxPercent(this.strVlToFloat(obj.value));
                    this.calculaPercentToValor();
                    var td_vlExec = this.objLinhaTr.find('td:eq(14)');
                    var input = td_vlExec.find('input');
                    input.val(this.vlrItemObraExec);
                    obj.value = this.floatToStr(this.percentItemObraExec);
                    break;
                case 'vl':
                    if(this.objCampoEditado.val().trim() === ''){
                        this.objCampoEditado.val('0,00');
                    }
                    this.vlrItemObraExec = this.strVlToFloat(obj.value);
                    this.validaMaxValor();
                    this.calculaValorToPercent();
                    var td_perExec = this.objLinhaTr.find('td:eq(13)');
                    var input = td_perExec.find('input');
                    input.val(this.percentItemObraExec);
                    break;
                default:
                    break;
            }
        }
        
        /* OK */
        if(this.tipoDado === 'DV'){
            switch(this.tipoCampo){
                case '%':
                    if(this.objCampoEditado.val().trim() === ''){
                        this.objCampoEditado.val('0,00');
                    }
                    this.percentItemObraExec = this.validaMaxPercent(this.strVlToFloat(this.objCampoEditado.val()));
                    this.calculaPercentToValor();
                    var td_vlValid = this.objLinhaTr.find('td:eq(18)');
                    var input = td_vlValid.find('input');
                    input.val(this.vlrItemObraExec);
                    this.objCampoEditado.val(this.floatToStr(this.percentItemObraExec));
                    break;
                case 'vl':
                    if(this.objCampoEditado.val().trim() === ''){
                        this.objCampoEditado.val('0,00');
                    }
                    this.vlrItemObraExec = this.strVlToFloat(this.objCampoEditado.val());
                    this.validaMaxValor();
                    this.calculaValorToPercent();
                    var td_perValid = this.objLinhaTr.find('td:eq(17)');
                    var input = td_perValid.find('input');
                    input.val(this.percentItemObraExec);                    
                    break;
            }
        }
        
        /** OK */
        if(this.tipoDado == 'F'){
            if(this.tipoCampo == 'qtd'){
                this.objLinhaTotais = $('#linha_totais_f');
                if(this.objCampoEditado.val().trim() === ''){
                    this.objCampoEditado.val('0,00');
                }
                this.qtdItemObraExec = this.strVlToFloat(this.objCampoEditado.val());
                this.calculaQtdToValores();
            }
        }
        
        /* OK */
        if(this.tipoDado === 'FV'){
            if(this.tipoCampo == 'qtd'){
                this.objLinhaTotais = $('#linha_totais_f');
                if(this.objCampoEditado.val().trim() === ''){
                    this.objCampoEditado.val('0,00');
                }
                this.qtdItemObraValid = this.strVlToFloat(this.objCampoEditado.val());
                this.calculaQtdToValoresValid();
            }
        }
        
        this.ajustaPercentItemSobreObra();
        this.atualizaLinhasTotais();
        
    };
    /** OK **/
    this.resetAtributos = function(){
        
        this.objTbodyCalc = null;
        this.objLinhaTr = null;
        this.objLinhaTotais = null;
        this.objCampoEditado = null;

        this.vlrObra = null;
        this.vlrItemObra = null;
        this.percentItemObra = null;

        this.qtdItemObraExec = null;
        this.vlrItemObraExec = null;
        this.percentItemObraExec = null;

        this.qtdItemObraValid = null;
        this.vlrItemObraValid = null;
        this.percentItemObraValid = null;

        this.valorItemObraExecSobreObra = null;
        this.percentItemObraExecSobreObra = null;

        this.tipoDado = null;
        this.tipoCampo = null;
        
    };
    /** OK **/
    this.strVlToFloat = function(vl){
        if(typeof(vl) === 'string'){ 
            var valorFloat = 0;
            var strVl = vl.replace(/\./g,'');
            strVl = strVl.replace(/\,/g,'.');
            valorFloat = parseFloat(strVl);
        }else{
            valorFloat = parseFloat(vl);
        }        
        return valorFloat;
    };
    /** OK **/
    this.floatToStr = function(vl){        
        if(typeof(vl) === 'number'){            
            vl = vl.toFixed(2);
            var floatAtualizado = vl.toString().replace(/\./g,',');
            if(floatAtualizado.search(',') === -1){
               floatAtualizado = floatAtualizado+',00'; 
            }
        }else{
            floatAtualizado = vl;
        }

        return floatAtualizado;
    };
    /** OK **/
    this.validaMaxPercent = function(vlPercent){
        var maxSaldo = this.strVlToFloat(this.objLinhaTr.find('td:eq(9)').text());
        if(vlPercent > maxSaldo){
            vlPercent = maxSaldo;
        }
        if(vlPercent > 100){
            vlPercent = 100.00;
            vlPercent = vlPercent.toFixed(2);
        }
        return vlPercent;
    };
    
    this.validaMaxValor = function(){
        var maxSaldo = this.strVlToFloat(this.objLinhaTr.find('td:eq(10)').text());
        var valorItem = this.vlrItemObraExec;
        if(valorItem > maxSaldo){
            valorItem = maxSaldo;
        }
        this.objCampoEditado.val(this.floatToStr(valorItem));
        this.vlrItemObraExec = valorItem;
    };
    
    /** OK **/
    this.calculaPercentToValor = function (){
        var valorObra     = 0;
        var valorItem     = 0;
        var valorExec     = 0;
        var valorExecObra = 0;
        var strVl         = '';
        
        valorItem = this.vlrItemObra;
        valorExec = (valorItem*this.percentItemObraExec)/100;
        valorExec = valorExec.toFixed(2);
        strVl     = valorExec.toString().replace(/\./g,',');
        
        if(strVl == 'NaN'){
           strVl = '0,00'; 
        }        
        this.vlrItemObraExec = strVl;
    };
    /** OK **/
    this.calculaValorToPercent = function (){
        
        var valorItem   = this.vlrItemObra;
        
        if(this.vlrItemObraExec > valorItem){
            this.objCampoEditado.val(this.floatToStr(valorItem));
            this.vlrItemObraExec = valorItem;
        }
        
        var percentExec = 0;
        var strPercent  = '';
        percentExec = (this.vlrItemObraExec*100)/valorItem;
        percentExec = percentExec.toFixed(2);
        strPercent = percentExec.toString().replace(/\./g,',');
        if(strPercent == 'NaN'){
           strPercent = '0,00'; 
        }        
        this.percentItemObraExec = strPercent;
    };
    //Cadastro/Edição/** OK **/
    this.calculaQtdToValores = function (){
        var qtd          = this.qtdItemObraExec;
        var vlTotalItens = this.strVlToFloat(this.objLinhaTotais.find('th:eq(1)').text());
        //Objetos - Células
        var td_valorItem            = this.objLinhaTr.find('td:eq(1)');
        var td_valorItemUnit        = this.objLinhaTr.find('td:eq(2)');
        var td_percentItemObra      = this.objLinhaTr.find('td:eq(3)');
        var td_qtdItem              = this.objLinhaTr.find('td:eq(4)');
        
        var td_percentExec          = this.objLinhaTr.find('td:eq(13)');
        var td_valorExec            = this.objLinhaTr.find('td:eq(15)');
        var td_percentExecSobreObra = this.objLinhaTr.find('td:eq(16)');
        //Valores
        var valorItem            = this.strVlToFloat(td_valorItem.text());
        var valorItemUnit        = this.strVlToFloat(td_valorItemUnit.text());
        var percentItemObra      = this.strVlToFloat(td_percentItemObra.text());
        var qtdItem              = this.strVlToFloat(td_qtdItem.text());
        
        var percentExec          = 0;
        var valorExec            = 0;
        var percentExecSobreObra = 0;
        
        if(qtd > qtdItem){
           qtd = qtdItem; 
           this.objCampoEditado.val(this.floatToStr(qtd));
        }

        if(qtdItem !== 0){
            valorExec = (qtd*valorItemUnit);
        }else{
            valorExec = 0;
        }
        
        var maxSaldoVl      = this.strVlToFloat(this.objLinhaTr.find('td:eq(12)').text());
        
        if(valorExec > maxSaldoVl){
            valorExec = maxSaldoVl;
            qtd = valorExec/valorItemUnit;
            this.objCampoEditado.val(this.floatToStr(qtd));
        }
        
        if(valorItem !== 0){
            percentExec = (valorExec*100)/valorItem;
        }else{
            percentExec = 0;
        }
        
        if(percentExec > 100){
            percentExec = 100.00;
        }
     
        td_percentExec.find('input').val(this.floatToStr(percentExec));
        td_percentExec.find('span').text(this.floatToStr(percentExec));
        
        td_valorExec.find('span:eq(0)').text(this.floatToStr(valorExec));
        td_valorExec.find('input').val(this.floatToStr(valorExec));
        
        if(qtd != 0){
            percentExecSobreObra = (valorExec*percentItemObra)/valorItem;
        }else{
            percentExecSobreObra = 0;
        }
        
        td_percentExecSobreObra.text(this.floatToStr(percentExecSobreObra));
        
        this.qtdItemObraExec = qtd;
        this.objCampoEditado.val(this.floatToStr(qtd));
        
    };
    
    //Validação - Cadastro/Edição/** OK **/
    this.calculaQtdToValoresValid = function (){
        
        var qtd          = this.qtdItemObraValid;
        var vlTotalItens = this.strVlToFloat(this.objLinhaTotais.find('th:eq(1)').text());
        //Objetos - Células
        var td_valorItem             = this.objLinhaTr.find('td:eq(1)');
        var td_percentItemObra       = this.objLinhaTr.find('td:eq(3)');
        var td_qtdItem               = this.objLinhaTr.find('td:eq(4)'); 
        
        var td_valorValid            = this.objLinhaTr.find('td:eq(17)');
        var td_percentValidSobreObra = this.objLinhaTr.find('td:eq(18)');
        
        //Valores
        var valorItem            = this.strVlToFloat(td_valorItem.text());
        var percentItemObra      = this.strVlToFloat(td_percentItemObra.text());
        var qtdItem              = this.strVlToFloat(td_qtdItem.text());
        
        var valorValid            = 0;
        var percentExecSobreObra = 0;
        
        if(qtd > qtdItem){
           qtd = qtdItem; 
           this.objCampoEditado.val(this.floatToStr(qtd));
        }        
        if(qtdItem !== 0){
            valorValid = (qtd*valorItem)/qtdItem;
        }else{
            valorValid = 0;
        }
        
        var td_valorItemUnit = this.objLinhaTr.find('td:eq(2)');
        var valorItemUnit    = this.strVlToFloat(td_valorItemUnit.text());
        
        var maxSaldoVl      = this.strVlToFloat(this.objLinhaTr.find('td:eq(12)').text());
        
        if(valorValid > maxSaldoVl){
            valorValid = maxSaldoVl;
            qtd = valorValid/valorItemUnit;
            this.objCampoEditado.val(this.floatToStr(qtd));
        }
        
        td_valorValid.find('span:eq(0)').text(this.floatToStr(valorValid));
        td_valorValid.find('input').val(this.floatToStr(valorValid));
        
        
        percentExecSobreObra = (valorValid*percentItemObra)/valorItem;

        var percentAcumulado = this.strVlToFloat(this.objLinhaTr.find('td:eq(9)').text());
        
        var percentSobreObraPosSup = percentAcumulado + percentExecSobreObra;
        
        var percentExec = (valorValid*100)/valorItem;

        td_percentValidSobreObra.find('span:eq(0)').text(this.floatToStr(percentSobreObraPosSup));
        td_percentValidSobreObra.find('input').val(this.floatToStr(percentExec));
        
        
    };
    /** OK **/
    this.calculaLinhaTotaisD = function (){
       
       /**
        * 
        * 1- Soma as colunas de grana
        * 2- NÂO somar os percentuais
        * 3- Calcular os percentuais totais com base nas granas, vl_obra = 100%, vl_sum_evolucao = x% => x = vl_sum_evolucao*100/vl_obra
        * 4- Atualizar as colunas de porcentagem
        */
        
        var linha;
        var sum_coluna01 = 0;      //Coluna 1 - Valor R$
        var sum_coluna02 = 100.00; //Coluna 2 - % Sobre a Obra
        var sum_coluna08 = 0;      //Coluna 8 - Última Supervisão - (%) do Item já executado sobre a Obra	
        var sum_coluna10 = 0;      //Coluna 10 - Supervisão Atual -  Valor Executado
        var sum_coluna11 = 0;      //Coluna 11 - Supervisão Atual - (%) do Item já executado sobre a Obra após Supervisão
        var sum_coluna112 = 0;     //Coluna 11 - Supervisão Atual - (%) do Item já executado sobre a Obra após Supervisão (levando em conta o acumulado)
        var sum_coluna13 = 0;      //Coluna 13 - Supervisão Atual - (%) executado
        var sum_coluna14 = 0;      //Coluna 14 - Supervisão Atual - Valor Executado
        var objCalc      = this;
        
        $('#'+this.objTbodyCalc.attr('id')+' > tr').each(function( index ) {
            linha = $(this);
            if(linha.attr('id') !== 'linha_totais_d'){
                //Soma os valores dos itens
                sum_coluna01 += (linha.find('td:eq(1)').text().trim()  !== '') ? objCalc.strVlToFloat(linha.find('td:eq(1)').text()) : 0;
                //Soma os % acumulado dos itens
                sum_coluna08 += (linha.find('td:eq(8)').text().trim()  !== '') ? objCalc.strVlToFloat(linha.find('td:eq(8)').text()) : 0;
                //Soma os valores do saldo
                sum_coluna10 += (linha.find('td:eq(10)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(10)').text()) : 0
                //Soma os valores dos itens
                if(typeof(linha.find('td:eq(14)').find('input').val()) !== 'undefined'){
                    sum_coluna14 += (linha.find('td:eq(14)').find('input').val().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(14)').find('input').val()) : 0;
                }
                
                //Não utilizado. Soma os percentuais dos itens sobre a obra após a supervisão. (levando em conta o acumulado)
                sum_coluna112 += (linha.find('td:eq(11)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(11)').text()) : 0;
            }
        });
        
        // % da supervisão sem levar em conta a soma com os valores acumulados
        sum_coluna11 = (sum_coluna14*100)/sum_coluna01;
        
        this.objLinhaTotais.find('th:eq(1)').html(this.floatToStr(sum_coluna01));
        this.objLinhaTotais.find('th:eq(2)').html(this.floatToStr(sum_coluna02));
        this.objLinhaTotais.find('th:eq(8)').html(this.floatToStr(sum_coluna08));
        this.objLinhaTotais.find('th:eq(10)').html(this.floatToStr(sum_coluna10));
        this.objLinhaTotais.find('th:eq(11)').html(this.floatToStr(sum_coluna11));
//        this.objLinhaTotais.find('th:eq(11)').html(this.floatToStr(sum_coluna11)+' - '+this.floatToStr(sum_coluna112)); // A diferença, caso exista, se dá por conta da soma do percentual acumulado com o percentual da evolução
        this.objLinhaTotais.find('th:eq(14)').html(this.floatToStr(sum_coluna14));
        
    };
    /** OK **/
    this.calculaLinhaTotaisDV = function (){
        
        var linha;
        var sum_coluna01 = 0;//Coluna 1  - Valor R$
        var sum_coluna02 = 100.00;//Coluna 2  - % Sobre a Obra
        var sum_coluna08 = 0;//Coluna 8  - Execução Acumulada - (%) do Item já executado sobre a Obra
        var sum_coluna10 = 0;//Coluna 10 - Saldo a executar   -  Valor Executado
        var sum_coluna11 = 0;//Coluna 11 - Supervisão Atual   - (%) do Item já executado sobre a Obra após Supervisão
        var sum_coluna13 = 0;//Coluna 13 - Supervisão Atual - (%) executado
        var sum_coluna14 = 0;//Coluna 14 - Supervisão Atual - Valor Executado
        var sum_coluna15 = 0;//Coluna 15 - Supervisão Validação - (%) do Item já executado sobre a Obra após Supervisão
        var sum_coluna18 = 0;//Coluna 18 - Supervisão Atual - Valor Executado
        var objCalc      = this;
        
        $('#'+this.objTbodyCalc.attr('id')+' > tr').each(function( index ) {
            linha = $(this);
            if(linha.attr('id') !== 'linha_totais_d'){
                //Soma os valores dos itens
                sum_coluna01 +=  (linha.find('td:eq(1)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(1)').text()) : 0;
                //Soma dos valores percentuais acumulado dos itens
                sum_coluna08 += (linha.find('td:eq(8)').text().trim() !== '')  ? objCalc.strVlToFloat(linha.find('td:eq(8)').text()) : 0;
                //Soma os valores do saldo a executar
                sum_coluna10 += (linha.find('td:eq(10)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(10)').text()) : 0;
                //Soma os valores dos itens exec atual
                sum_coluna14 += (linha.find('td:eq(14)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(14)').text()) : 0;
                //Soma os valores dos itens validação
                if(typeof(linha.find('td:eq(18)').find('input').val()) !== 'undefined'){
                    sum_coluna18 += (linha.find('td:eq(18)').find('input').val().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(18)').find('input').val()) : 0;
                }
            }
        });
        
        // % da evolução sem levar em conta a soma com os valores acumulados
        sum_coluna11 = (sum_coluna14*100)/sum_coluna01;
        
        // % da validação sem levar em conta a soma com os valores acumulados
        sum_coluna15 = (sum_coluna18*100)/sum_coluna01;
        
        this.objLinhaTotais.find('th:eq(1)').html(this.floatToStr(sum_coluna01));
        this.objLinhaTotais.find('th:eq(2)').html(this.floatToStr(sum_coluna02));
        this.objLinhaTotais.find('th:eq(8)').html(this.floatToStr(sum_coluna08));
        this.objLinhaTotais.find('th:eq(10)').html(this.floatToStr(sum_coluna10));
        this.objLinhaTotais.find('th:eq(11)').html(this.floatToStr(sum_coluna11));
        this.objLinhaTotais.find('th:eq(14)').html(this.floatToStr(sum_coluna14));
        this.objLinhaTotais.find('th:eq(15)').html(this.floatToStr(sum_coluna15));
        this.objLinhaTotais.find('th:eq(18)').html(this.floatToStr(sum_coluna18));
        
    };
    /** OK **/
    this.calculaLinhaTotaisF = function (){
        
        /**
        * 
        * 1- Soma as colunas de grana
        * 2- NÂO somar os percentuais
        * 3- Calcular os percentuais totais com base nas granas, vl_obra = 100%, vl_sum_evolucao = x% => x = vl_sum_evolucao*100/vl_obra
        * 4- Atualizar as colunas de porcentagem
        */
       
        var linha;
        var sum_coluna01 = 0;//Coluna 1  - Valor R$
        var sum_coluna03 = 100.00;//Coluna 2  - % Sobre a Obra
        var sum_coluna09 = 0;
        var sum_coluna12 = 0;
        var sum_coluna15 = 0;
        var sum_coluna16 = 0;
        var sum_coluna162 = 0;
        var objCalc      = this;
        
        $('#'+this.objTbodyCalc.attr('id')+' > tr').each(function( index ) {
            linha = $(this);
            if(linha.attr('id') !== 'linha_totais_f'){
                //Soma os valores dos itens
                sum_coluna01 += (linha.find('td:eq(1)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(1)').text()) : 0;
                //Soma dos valores percentuais acumulado dos itens
                sum_coluna09 += (linha.find('td:eq(9)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(9)').text()) : 0;
                //Valores de saldo a executar
                sum_coluna12 += (linha.find('td:eq(12)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(12)').text()) : 0;
                //Soma os valores dos itens da evolução atual
                sum_coluna15 += (linha.find('td:eq(15)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(15)').text()) : 0;
                
                sum_coluna162 += (linha.find('td:eq(16)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(16)').text()) : 0;
            }
        });
        
        sum_coluna16 = (sum_coluna15*100)/sum_coluna01;
        
        this.objLinhaTotais.find('th:eq(1)').html(this.floatToStr(sum_coluna01));
        this.objLinhaTotais.find('th:eq(3)').html(this.floatToStr(sum_coluna03));
        this.objLinhaTotais.find('th:eq(9)').html(this.floatToStr(sum_coluna09));
        this.objLinhaTotais.find('th:eq(12)').html(this.floatToStr(sum_coluna12));
        this.objLinhaTotais.find('th:eq(15)').html(this.floatToStr(sum_coluna15));
        this.objLinhaTotais.find('th:eq(16)').html(this.floatToStr(sum_coluna16));
    };
    /** OK **/
    this.calculaLinhaTotaisFV = function (){
        
        var linha;
        var sum_coluna01 = 0;
        var sum_coluna03 = 100.00;
        var sum_coluna09 = 0;
        var sum_coluna12 = 0;
        var sum_coluna14 = 0;
        var sum_coluna15 = 0;
        var sum_coluna17 = 0;
        var sum_coluna18 = 0;
        var objCalc      = this;
        
        $('#'+this.objTbodyCalc.attr('id')+' > tr').each(function( index ) {
            linha = $(this);
            if(linha.attr('id') !== 'linha_totais_f_v'){
                //Soma os valores dos itens
                sum_coluna01 += (linha.find('td:eq(1)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(1)').text()) : 0;
                //Soma dos valores percentuais acumulado dos itens
                sum_coluna09 += (linha.find('td:eq(9)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(9)').text()) : 0;
                // Valor saldo a executar
                sum_coluna12 += (linha.find('td:eq(12)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(12)').text()) : 0;
                // Soma os Valores da validação atual 
                sum_coluna14 += (linha.find('td:eq(14)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(14)').text()) : 0;
                // Soma os Valores da validação atual 
                sum_coluna17 += (linha.find('td:eq(17)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(17)').text()) : 0;
            }
        });
        
        sum_coluna15 = ((sum_coluna14*100)/sum_coluna01);
        sum_coluna18 = ((sum_coluna17*100)/sum_coluna01);
        
        this.objLinhaTotais.find('th:eq(1)').html(this.floatToStr(sum_coluna01));
        this.objLinhaTotais.find('th:eq(3)').html(this.floatToStr(sum_coluna03));
        this.objLinhaTotais.find('th:eq(9)').html(this.floatToStr(sum_coluna09));
        this.objLinhaTotais.find('th:eq(12)').html(this.floatToStr(sum_coluna12));
        this.objLinhaTotais.find('th:eq(14)').html(this.floatToStr(sum_coluna14));
        this.objLinhaTotais.find('th:eq(15)').html(this.floatToStr(sum_coluna15));
        this.objLinhaTotais.find('th:eq(17)').html(this.floatToStr(sum_coluna17));
        this.objLinhaTotais.find('th:eq(18)').html(this.floatToStr(sum_coluna18));
        
    };
    /* OK */
    this.validaValoresEvolucaoMI = function (){
        
        var objRetorno = { diff: false , msg : '' };
        
        //Calculado
        var emipecentmedidoedif_sum  = 0;
        var emivlmedidoedif_sum      = 0;
        var emipecentmedidoext_sum   = 0;
        var emivlmedidoext_sum       = 0;
        //Campos da validação
        var emipecentvalidadoedif_sum = 0;
        var emivlvalidadoedif_sum     = 0;
        var emipecentvalidadoext_sum  = 0;
        var emivlvalidadoext_sum      = 0;
        //Informado
        var emipecentmedidoedif_info  = 0;
        var emivlmedidoedif_info      = 0;        
        var emipecentmedidoext_info   = 0;
        var emivlmedidoext_info       = 0;
        //Campos da validação
        var emipecentvalidadoedif_info = 0;
        var emivlvalidadoedif_info     = 0;
        var emipecentvalidadoext_info  = 0;
        var emivlvalidadoext_info      = 0;
        
        var diff                       = 0;
        
        if($('#linha_totais_d').length > 0){
            this.objLinhaTotais = $('#linha_totais_d');
            this.objTbodyCalc   = this.objLinhaTotais.parents('tbody');
            
            emipecentmedidoedif_sum = this.strVlToFloat(this.objLinhaTotais.find('th:eq(11)').html());
            emivlmedidoedif_sum     = this.strVlToFloat(this.objLinhaTotais.find('th:eq(14)').html());
            
            emipecentmedidoedif_info  = this.strVlToFloat($("input[name='emipecentmedidoedif']").val());
            emivlmedidoedif_info      = this.strVlToFloat($("input[name='emivlmedidoedif']").val());
            
            diff = emipecentmedidoedif_sum - emipecentmedidoedif_info;
            if(diff !== 0 && emipecentmedidoedif_info !== 0){
                objRetorno.msg += '\n - Existe uma diferença entre o percentual dos itens de edificação medido e o informado. Diferença de: '+this.floatToStr(diff);
                objRetorno.diff = true;
            }
            
            diff = 0;
            diff = emivlmedidoedif_sum - emivlmedidoedif_info;
            if(diff !== 0 && emivlmedidoedif_info !== 0){
                objRetorno.msg += '\n - Existe uma diferença entre o valor dos itens de edificação medido e o informado. Diferença de: '+this.floatToStr(diff);
                objRetorno.diff = true;
            }
        }
        
        diff = 0;
        if($('#linha_totais_d_v').length > 0){
            this.objLinhaTotais = $('#linha_totais_d_v');
            this.objTbodyCalc   = this.objLinhaTotais.parents('tbody');
            
            emipecentvalidadoedif_sum = this.strVlToFloat(this.objLinhaTotais.find('th:eq(15)').html());
            emivlvalidadoedif_sum     = this.strVlToFloat(this.objLinhaTotais.find('th:eq(18)').html());
            
            emipecentvalidadoedif_info  = this.strVlToFloat($("input[name='emipecentvalidadoedif']").val());
            emivlvalidadoedif_info      = this.strVlToFloat($("input[name='emivlvalidadoedif']").val());
            
            diff = emipecentvalidadoedif_sum - emipecentvalidadoedif_info;
            if(diff !== 0 && emipecentvalidadoedif_info !== 0){
                objRetorno.msg += '\n - Existe uma diferença entre o percentual dos itens de edificação validado e o informado. Diferença de: '+this.floatToStr(diff);
                objRetorno.diff = true;
            }
            
            diff = 0;
            diff = emivlvalidadoedif_sum - emivlvalidadoedif_info;
            if(diff !== 0 && emivlvalidadoedif_info !== 0){
                objRetorno.msg += '\n - Existe uma diferença entre o valor dos itens de edificação validado e o informado. Diferença de: '+this.floatToStr(diff);
                objRetorno.diff = true;
            }
        }
        
        diff = 0;
        if($('#linha_totais_f').length > 0){
            this.objLinhaTotais = $('#linha_totais_f');
            this.objTbodyCalc   = this.objLinhaTotais.parents('tbody');
            
            emipecentmedidoext_sum = this.strVlToFloat(this.objLinhaTotais.find('th:eq(16)').html());
            emivlmedidoext_sum     = this.strVlToFloat(this.objLinhaTotais.find('th:eq(15)').html());
            
            emipecentmedidoext_info  = this.strVlToFloat($("input[name='emipecentmedidoext']").val());
            emivlmedidoext_info      = this.strVlToFloat($("input[name='emivlmedidoext']").val());
            
            diff = emipecentmedidoext_sum - emipecentmedidoext_info;
            if(diff !== 0 && emipecentmedidoext_info !== 0){
                objRetorno.msg += '\n - Existe uma diferença entre o percentual dos itens de Serviços Externos medido e o informado. Diferença de: '+this.floatToStr(diff);
                objRetorno.diff = true;
            }
            
            diff = 0;
            diff = emivlmedidoext_sum - emivlmedidoext_info;
            if(diff !== 0 && emivlmedidoext_info !== 0){
                objRetorno.msg += '\n - Existe uma diferença entre o valor dos itens de Serviços Externos medido e o informado. Diferença de: '+this.floatToStr(diff);
                objRetorno.diff = true;
            }
            
        }
        
        diff = 0;
        if($('#linha_totais_f_v').length > 0){
            this.objLinhaTotais = $('#linha_totais_f_v');
            this.objTbodyCalc   = this.objLinhaTotais.parents('tbody');
            
            emipecentvalidadoext_sum = this.strVlToFloat(this.objLinhaTotais.find('th:eq(18)').html());
            emivlvalidadoext_sum     = this.strVlToFloat(this.objLinhaTotais.find('th:eq(17)').html());
            
            emipecentvalidadoext_info  = this.strVlToFloat($("input[name='emipecentvalidadoext']").val());
            emivlvalidadoext_info      = this.strVlToFloat($("input[name='emivlvalidadoext']").val());
            
            diff = emipecentvalidadoext_sum - emipecentvalidadoext_info;
            if(diff !== 0 && emipecentvalidadoext_info !== 0){
                objRetorno.msg += '\n - Existe uma diferença entre o percentual dos itens de Serviços Externos validado e o informado. Diferença de: '+this.floatToStr(diff);
                objRetorno.diff = true;
            }
            
            diff = 0;
            diff = emivlvalidadoext_sum - emivlvalidadoext_info;
            if(diff !== 0 && emivlvalidadoext_info !== 0){
                objRetorno.msg += '\n - Existe uma diferença entre o valor dos itens de Serviços Externos validado e o informado. Diferença de: '+this.floatToStr(diff);
                objRetorno.diff = true;
            }
        }
        
        return objRetorno;

    };
    /* OK */
    this.ajustaValoresEvolucaoMI = function (){
        //Calculado
        var emipecentmedidoedif_sum  = 0;
        var emivlmedidoedif_sum      = 0;
        var emipecentmedidoext_sum   = 0;
        var emivlmedidoext_sum       = 0;
        //Campos da validação
        var emipecentvalidadoedif_sum = 0;
        var emivlvalidadoedif_sum     = 0;
        var emipecentvalidadoext_sum  = 0;
        var emivlvalidadoext_sum      = 0;
        
        if($('#linha_totais_d').length > 0){
            this.objLinhaTotais     = $('#linha_totais_d');
            this.objTbodyCalc       = this.objLinhaTotais.parents('tbody');
            emipecentmedidoedif_sum = (this.objLinhaTotais.find('th:eq(11)').html());
            emivlmedidoedif_sum     = (this.objLinhaTotais.find('th:eq(14)').html());
            $("input[name='emipecentmedidoedif']").val(emipecentmedidoedif_sum);
            $("input[name='emivlmedidoedif']").val(emivlmedidoedif_sum);
            $("input[name='emivlmedidoedif']").focus();
        }
        
        if($('#linha_totais_d_v').length > 0){
            this.objLinhaTotais       = $('#linha_totais_d_v');
            this.objTbodyCalc         = this.objLinhaTotais.parents('tbody');            
            emipecentvalidadoedif_sum = (this.objLinhaTotais.find('th:eq(15)').html());
            emivlvalidadoedif_sum     = (this.objLinhaTotais.find('th:eq(18)').html());
            $("input[name='emipecentvalidadoedif']").val(emipecentvalidadoedif_sum);
            $("input[name='emivlvalidadoedif']").val(emivlvalidadoedif_sum);
            $("input[name='emivlvalidadoedif']").focus();
        }
        
        if($('#linha_totais_f').length > 0){
            this.objLinhaTotais    = $('#linha_totais_f');
            this.objTbodyCalc      = this.objLinhaTotais.parents('tbody');
            emipecentmedidoext_sum = (this.objLinhaTotais.find('th:eq(16)').html());
            emivlmedidoext_sum     = (this.objLinhaTotais.find('th:eq(15)').html());
            $("input[name='emipecentmedidoext']").val(emipecentmedidoext_sum);
            $("input[name='emivlmedidoext']").val(emivlmedidoext_sum);
            $("input[name='emivlmedidoext']").focus();
        }
        
        if($('#linha_totais_f_v').length > 0){
            this.objLinhaTotais      = $('#linha_totais_f_v');
            this.objTbodyCalc        = this.objLinhaTotais.parents('tbody');
            emipecentvalidadoext_sum = (this.objLinhaTotais.find('th:eq(18)').html());
            emivlvalidadoext_sum     = (this.objLinhaTotais.find('th:eq(17)').html());
            $("input[name='emipecentvalidadoext']").val(emipecentvalidadoext_sum);
            $("input[name='emivlvalidadoext']").val(emivlvalidadoext_sum);
            $("input[name='emivlvalidadoext']").focus();
        }
    };
    /** OK **/
    this.ajustaPercentItemSobreObra = function(){
        
        var vlItem   = 0;
        var percItem = 0;
        var qtdItem  = 0;
        
        var percItemAcumuladoExec      = 0;
        var percItemAcumuladoSobreObra = 0;
        var vlItemAcumulado            = 0;
        var qtdItemAcumulado           = 0;
        
        var vlItemExec   = 0;
        var percItemExec = 0;
        var qtdItemExec  = 0;
        
        var percItemSobreObra       = 0;
        var percItemSobreObraPosSup = 0;
        
        switch(this.tipoDado){
            case 'D':
                vlItem   = this.strVlToFloat(this.objLinhaTr.find('td:eq(1)').text());
                percItem = this.strVlToFloat(this.objLinhaTr.find('td:eq(2)').text());
                qtdItem  = (this.objLinhaTr.find('td:eq(3)').text().trim() === '-') ? 0.00 : this.strVlToFloat(this.objLinhaTr.find('td:eq(3)').text());

                percItemAcumuladoExec      = this.strVlToFloat(this.objLinhaTr.find('td:eq(7)').text());
                percItemAcumuladoSobreObra = this.strVlToFloat(this.objLinhaTr.find('td:eq(8)').text());
                
                vlItemAcumulado            = (vlItem*percItemAcumuladoExec)/100;
                qtdItemAcumulado           = (vlItemAcumulado*qtdItem)/vlItem;
        
                percItemExec = this.strVlToFloat(this.objLinhaTr.find('td:eq(13)').find('input').val());
                vlItemExec   = this.strVlToFloat(this.objLinhaTr.find('td:eq(14)').find('input').val());
                qtdItemExec  = 0;
                
                if(vlItem !== 0 && vlItemExec !== 0){
                    percItemSobreObra       = (vlItemExec*percItem)/vlItem;
                    percItemSobreObraPosSup = percItemAcumuladoSobreObra+percItemSobreObra;
                }else{
                    percItemSobreObra       = 0;
                    percItemSobreObraPosSup = 0;
                }
                
                this.objLinhaTr.find('td:eq(11)').text(this.floatToStr(percItemSobreObraPosSup));
                
                break;
            case 'DV':
                vlItem   = this.strVlToFloat(this.objLinhaTr.find('td:eq(1)').text());
                percItem = this.strVlToFloat(this.objLinhaTr.find('td:eq(2)').text());
                qtdItem  = (this.objLinhaTr.find('td:eq(3)').text().trim() === '-') ? 0.00 : this.strVlToFloat(this.objLinhaTr.find('td:eq(3)').text());

                percItemAcumuladoExec      = this.strVlToFloat(this.objLinhaTr.find('td:eq(7)').text());
                percItemAcumuladoSobreObra = this.strVlToFloat(this.objLinhaTr.find('td:eq(8)').text());
                vlItemAcumulado            = (vlItem*percItemAcumuladoExec)/100;
                qtdItemAcumulado           = (vlItemAcumulado*qtdItem)/vlItem;
                
                percItemExec = this.strVlToFloat(this.objLinhaTr.find('td:eq(17)').find('input').val());
                vlItemExec   = this.strVlToFloat(this.objLinhaTr.find('td:eq(18)').find('input').val());
                qtdItemExec  = 0;
                
                if(vlItem !== 0 && vlItemExec !== 0){
                    percItemSobreObra       = (vlItemExec*percItem)/vlItem;
                    percItemSobreObraPosSup = percItemAcumuladoSobreObra+percItemSobreObra;
                }else{
                    percItemSobreObra       = 0;
                    percItemSobreObraPosSup = 0;
                }
                
                this.objLinhaTr.find('td:eq(15)').text(this.floatToStr(percItemSobreObraPosSup));
                break;
            case 'F':
                
                vlItem   = this.strVlToFloat(this.objLinhaTr.find('td:eq(1)').text());
                percItem = this.strVlToFloat(this.objLinhaTr.find('td:eq(3)').text());
                qtdItem  = (this.objLinhaTr.find('td:eq(4)').text().trim() === '-') ? 0.00 : this.strVlToFloat(this.objLinhaTr.find('td:eq(4)').text());

                percItemAcumuladoExec      = this.strVlToFloat(this.objLinhaTr.find('td:eq(8)').text());
                percItemAcumuladoSobreObra = this.strVlToFloat(this.objLinhaTr.find('td:eq(9)').text());
                vlItemAcumulado            = (vlItem*percItemAcumuladoExec)/100;
                qtdItemAcumulado           = (vlItemAcumulado*qtdItem)/vlItem;
                
                qtdItemExec  = this.qtdItemObraExec;
                vlItemExec   = this.strVlToFloat(this.objLinhaTr.find('td:eq(15)').text());
                
                if(vlItem !== 0){
                    percItemExec      = (vlItemExec*100)/vlItem;
                    percItemSobreObra = (vlItemExec*percItem)/vlItem;
                }else{
                    percItemExec      = 0;
                    percItemSobreObra = 0;
                }
                
                if(this.qtdItemObraExec !== 0.00 && this.qtdItemObraExec !== 0){
                    percItemSobreObraPosSup = percItemAcumuladoSobreObra+percItemSobreObra;
                }else{
                    percItemSobreObraPosSup = 0;
                }
                
                this.objLinhaTr.find('td:eq(16)').text(this.floatToStr(percItemSobreObraPosSup));
                
                break;
            case 'FV':
                
                vlItem   = this.strVlToFloat(this.objLinhaTr.find('td:eq(1)').text());
                percItem = this.strVlToFloat(this.objLinhaTr.find('td:eq(3)').text());
                qtdItem  = (this.objLinhaTr.find('td:eq(4)').text().trim() === '-') ? 0.00 : this.strVlToFloat(this.objLinhaTr.find('td:eq(4)').text());

                percItemAcumuladoExec      = this.strVlToFloat(this.objLinhaTr.find('td:eq(8)').text());
                percItemAcumuladoSobreObra = this.strVlToFloat(this.objLinhaTr.find('td:eq(9)').text());
                vlItemAcumulado            = (vlItem*percItemAcumuladoExec)/100;
                qtdItemAcumulado           = (vlItemAcumulado*qtdItem)/vlItem;
                
                var td_percentValidSobreObra = this.objLinhaTr.find('td:eq(18)');
                
                qtdItemExec  = this.qtdItemObraValid;
                if(qtdItem !== 0){
                    vlItemExec = (qtdItemExec*vlItem)/qtdItem;
                }else{
                    vlItemExec = 0;
                }        
                if(vlItem !== 0 && vlItemExec !== 0){
                    percItemExec      = (vlItemExec*100)/vlItem;
                    percItemSobreObra = (vlItemExec*percItem)/vlItem;
                    percItemSobreObraPosSup = percItemAcumuladoSobreObra+percItemSobreObra;
                }else{
                    percItemExec      = 0;
                    percItemSobreObra = 0;
                    percItemSobreObraPosSup = 0;
                }
                
                td_percentValidSobreObra.find('span:eq(0)').text(this.floatToStr(percItemSobreObraPosSup));
                td_percentValidSobreObra.find('input').val(this.floatToStr(percItemExec));
                
                break;
        }
        
        
    };
    /* OK */
    this.atualizaLinhasTotais = function (){
        if($('#linha_totais_d').length > 0){
            this.objLinhaTotais = $('#linha_totais_d');
            this.objTbodyCalc   = this.objLinhaTotais.parents('tbody');
            this.calculaLinhaTotaisD();
        }
        if($('#linha_totais_f').length > 0){
            this.objLinhaTotais = $('#linha_totais_f');
            this.objTbodyCalc   = this.objLinhaTotais.parents('tbody');
            this.calculaLinhaTotaisF();
        }
        if($('#linha_totais_d_v').length > 0){
            this.objLinhaTotais = $('#linha_totais_d_v');
            this.objTbodyCalc   = this.objLinhaTotais.parents('tbody');
            this.calculaLinhaTotaisDV();
        }
        if($('#linha_totais_f_v').length > 0){
            this.objLinhaTotais = $('#linha_totais_f_v');
            this.objTbodyCalc   = this.objLinhaTotais.parents('tbody');
            this.calculaLinhaTotaisFV();
        }
    };
}

var calculosEvolucao = new calculosEvolucao();

</script>

<script type="text/javascript">

function abrirGaleria(arqid, pagina){
    window.open("../slideshow/slideshow/obras2_galeriaGaleriaFotos.php?&arqid=" + arqid + "&emiid=<?php echo $emiid; ?>", "imagem", "width=850,height=600,resizable=yes");
}

<?php
    $arPflcod = array(PFLCOD_SUPERVISOR_MEC,
                      PFLCOD_ADMINISTRADOR,
                      PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                      PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
                      PFLCOD_SUPERVISOR_UNIDADE,
                      PFLCOD_GESTOR_UNIDADE,
                      PFLCOD_GESTOR_MEC,
                      PFLCOD_SUPER_USUARIO);
    if (possuiPerfil($arPflcod)) { ?>

function ImageComponent(params) {
    var a = window.open("plugins/component_foto.php" + params, "inserir_fotos", "scrollbars=yes,height=540,width=630");
    a.focus();
}

function inserirNovasFotos(){
    var emiid = $('#emiid').val();
    
    if(emiid != 0){
        window.location.href = 'obras2.php?modulo=principal/cadEvolucaoMi&acao=E&emiid='+emiid+'#divFotos';
        return true;
    }
    
    $('#req').val('cadastroNovaEvoFotos');
    var url   = window.location+'&req=cadastroNovaEvoFotos';
    var dados = $('#formulario').serialize();
    
    $.ajax({
            url: url,
            type: 'POST',
            data: dados,
            success: function(data) {
                if (data == "false"){
                    alert("Não foi possível realizar a operação!");
                } else{
                    if($.isNumeric( data )){
                        window.location.href = 'obras2.php?modulo=principal/cadEvolucaoMi&acao=E&emiid='+data+'#divFotos';
                    }else{
                        alert('Ocorreu um erro inesperado. Cadastre uma Evolução antes de adicionar novas fotos.');
                        console.info(data);
                    }
                }
            }
    });
    
    
}

function salvarFotos()
{
$("[name=btn_salvar_fotos]").val("Carregando...");
        $("[name=btn_salvar_fotos]").attr("disabled", "disabled");
        
        var arrFotosSupervisao = $("#fotos_supervisao").sortable("serialize");
        var arrFotosGaleria    = $("#fotos_galeria").sortable("serialize");
        var url                = window.location;
        var dados              = { ajaxFotosGaleria: true, arrFotosSupervisao: arrFotosSupervisao , arrFotosGaleria: arrFotosGaleria };

        $.ajax({
                url: url,
                type: 'POST',
                data: dados,
                success: function(data) {

                if (data == "true"){
                alert("Operação realizada com sucesso!");
                        $("[name=btn_salvar_fotos]").val("Salvar Fotos");
                        $("[name=btn_salvar_fotos]").removeAttr("disabled");
                } else{
                alert("Não foi possível realizar a operação!");
                        $("[name=btn_salvar_fotos]").val("Salvar Fotos");
                        $("[name=btn_salvar_fotos]").removeAttr("disabled");
                }
                }
        });
}   

function removerFotoGaleria(foto){
    jQuery("#s_" + foto).remove();
    jQuery("#" + foto).attr("class", "draggable");
}

<?php }else{ ?>
    
    function ImageComponent(params){
        alert('Você não possui permissão para acessar essa funcionalidade.');
    }
    
    function inserirNovasFotos(){
        alert('Você não possui permissão para acessar essa funcionalidade.');
    }
    
    function salvarFotos(){
        alert('Você não possui permissão para acessar essa funcionalidade.');
    }
    
    function removerFotoGaleria(foto){
        alert('Você não possui permissão para acessar essa funcionalidade.');
    }

<?php } ?>

jQuery(document).ready(function(){
    
    var bloqueiaForm      =  <?php echo $bloqueiaForm; ?>;
    var avisoBloqueioForm = '<?php echo $avisoBloqueioForm; ?>';
    
    verificaDataAcompanhamento = function(){
        var d       = new Date();
        var dt_evo  = jQuery('#emidtevolucao').val();
        dt_evo      = dt_evo.split('/');
        var dt_comp = new Date(dt_evo[2],dt_evo[1],dt_evo[0]).getTime();
        var dia = d.getDate();
        var mes = d.getMonth()+1;
        var ano = d.getFullYear();
        var dt_hj   = new Date(ano,mes,dia).getTime();
        if (new Date(dt_comp).getTime() > new Date(dt_hj).getTime()) {
            alert('A data do Acompanhamento não pode ser maior que a data de hoje.');
            jQuery('#emidtevolucao').val('');
//            jQuery('#emidtevolucao').focus();
            return false;
        }
        return true;
    };
    
    verificaDataPrevConclusao = function(){
        var d       = new Date();
        var dt_prev = jQuery('#emidtconclusaoprev').val();
        dt_prev     = dt_prev.split('/');
        var dt_comp = new Date(dt_prev[2],dt_prev[1],dt_prev[0]).getTime();
        var dt_evo  = jQuery('#emidtevolucao').val();
        dt_evo      = dt_evo.split('/');
        var dt_min  = new Date(dt_evo[2],dt_evo[1],dt_evo[0]).getTime();
        if (new Date(dt_min) > new Date(dt_comp)) {
            alert('A data da Prevista de Conclusão não pode ser menor que a data de Acompanhamento.');
            jQuery('#emidtconclusaoprev').val('');
            return false;
        }
        return true;
    };
    
    jQuery('#emidtevolucao').focusout(function(){
        verificaDataAcompanhamento();
    });
    
    jQuery('#emidtconclusaoprev').focusout(function(){
        verificaDataPrevConclusao();
    });
    
    if(bloqueiaForm === true){
        jQuery('#divForm').find('input, textarea, button, select').attr('disabled','disabled');
        if(avisoBloqueioForm !== ''){
            alert(avisoBloqueioForm);
        }            
        jQuery('#btn_voltar').attr('disabled', false);
        
        <?php
        
        $emiid = $_GET['emiid'];
        $evoMi    = new EvolucaoMi();
        $estado = $evoMi->getEstadoWfEvolucaoMI($emiid);
        
        if($estado['esdid'] != ESDID_MI_CONCLUIDO){ 
            echo "jQuery('#add_fotos').attr('disabled', false);
                  jQuery('#btn_salvar_fotos').attr('disabled', false);";
        }
        ?>
        
        
    }

    jQuery('#btn_salvar_evolucao').click(function(){
        
        if(jQuery('#req').val() == 'validacao'){
            
            if(jQuery('[name=emipecentvalidadoedif]').val().trim() == ''){
                alert('O campo "% Validado da Edificação" deve ser preenchido');
                jQuery('[name=emipecentvalidadoedif]').focus();
                return false;
            }
            else if(jQuery('[name=emipecentvalidadoext]').val().trim() == ''){
                alert('O campo "% Validado dos Serviços Externos" deve ser preenchido');
                jQuery('[name=emipecentvalidadoext]').focus();
                return false;
            }
            else if(jQuery('[name=emivlvalidadoedif]').val().trim() == ''){
                alert('O campo "Valor Validado da Edificação (R$)" deve ser preenchido');
                jQuery('[name=emivlvalidadoedif]').focus();
                return false;
            }
            else if(jQuery('[name=emivlvalidadoext]').val().trim() == ''){
                alert('O campo "Valor Validado dos Serviços Externos (R$)" deve ser preenchido');
                jQuery('[name=emivlvalidadoext]').focus();
                return false;
            }
            
            if(jQuery('[name=emivlvalidadoedif]').val() == '0,00' && jQuery('[name=emipecentvalidadoedif]').val() == '0,00'){
                calculosEvolucao.ajustaValoresEvolucaoMI();
            }
            
            if(jQuery('[name=emivlvalidadoext]').val() == '0,00' && jQuery('[name=emipecentvalidadoext]').val() == '0,00'){
                calculosEvolucao.ajustaValoresEvolucaoMI();
            }
            
            if(jQuery('[name=emivlvalidadoedif]').val() == '0,00' && jQuery('[name=emipecentvalidadoedif]').val() == '0,00'){
                var r0 = window.confirm('As medições de valor e percentual de Edificação estão com valor zero(0,00). Deseja atualizar os valores ?\nATENÇÂO: Se os valores não forem atualizados, eles serão lançados como ZERO.');
                
                if(r0 === true){
                    jQuery('[name=emivlvalidadoedif]').focus();
                    calculosEvolucao.ajustaValoresEvolucaoMI();
                    return false;
                }
            }
            
            if(jQuery('[name=emivlvalidadoext]').val() == '0,00' && jQuery('[name=emipecentvalidadoext]').val() == '0,00'){
                var r1 = window.confirm('As medições de valor e percentual dos Serviços Externos estão com valor zero(0,00). Deseja atualizar os valores ?\nATENÇÂO: Se os valores não forem atualizados, eles serão lançados como ZERO.');
                
                if(r1 === true){
                    jQuery('[name=emivlvalidadoext]').focus();
                    calculosEvolucao.ajustaValoresEvolucaoMI();
                    return false;
                }
                
            }
            
        }
        else{
            if(verificaDataAcompanhamento() === false){
                return false;
            }
            if(verificaDataPrevConclusao() === false){
                return false;
            }
                
            if(jQuery('[name=emidtevolucao]').val().trim() == ''){
                alert('O campo "Data do Acompanhamento" deve ser preenchido');
                jQuery('[name=emidtevolucao]').val('');
                jQuery('[name=emidtevolucao]').focus();
                return false;
            }
            else if(jQuery('[name=emidtconclusaoprev]').val().trim() == ''){
                alert('O campo "Data Prevista de Conclusão" deve ser preenchido');
                jQuery('[name=emidtconclusaoprev]').val('');
                jQuery('[name=emidtconclusaoprev]').focus();
                return false;
            }
            else if(jQuery('[name=emipecentmedidoedif]').val().trim() == ''){
                alert('O campo "% Aferido da Edificação" deve ser preenchido');
                jQuery('[name=emipecentmedidoedif]').focus();
                return false;
            }
            else if(jQuery('[name=emipecentmedidoext]').val().trim() == ''){
                alert('O campo "% Aferido dos Serviços Externos" deve ser preenchido');
                jQuery('[name=emipecentmedidoext]').focus();
                return false;
            }
            else if(jQuery('[name=emivlmedidoedif]').val().trim() == ''){
                alert('O campo "Valor Aferido da Edificação (R$)" deve ser preenchido');
                jQuery('[name=emivlmedidoedif]').focus();
                return false;
            }
            else if(jQuery('[name=emivlmedidoext]').val().trim() == ''){
                alert('O campo "Valor Aferido dos Serviços Externos (R$)" deve ser preenchido');
                jQuery('[name=emivlmedidoext]').focus();
                return false;
            }
            
            if(jQuery('[name=emivlmedidoedif]').val() == '0,00' && jQuery('[name=emipecentmedidoedif]').val() == '0,00'){
                var r0 = window.confirm('As medições de valor e percentual de Edificação estão com valor zero(0,00). Deseja atualizar os valores ?\nATENÇÂO: Se os valores não forem atualizados, eles serão lançados como ZERO.');
                
                if(r0 === true){
                    jQuery('[name=emivlmedidoedif]').focus();
                    calculosEvolucao.ajustaValoresEvolucaoMI();
                    return false;
                }                
            }
            
            if(jQuery('[name=emivlmedidoext]').val() == '0,00' && jQuery('[name=emipecentmedidoext]').val() == '0,00'){
                var r1 = window.confirm('As medições de valor e percentual dos Serviços Externos estão com valor zero(0,00). Deseja atualizar os valores ?\nATENÇÂO: Se os valores não forem atualizados, eles serão lançados como ZERO.');
                
                if(r1 === true){
                    jQuery('[name=emivlmedidoext]').focus();
                    calculosEvolucao.ajustaValoresEvolucaoMI();
                    return false;
                }
            }
        }
        
        var validDadosEvolucao = calculosEvolucao.validaValoresEvolucaoMI();
        if(validDadosEvolucao.diff !== false){
            alert('O valores das tabelas não estão de acordo com os valores informados.\n'+validDadosEvolucao.msg);
//            var r2 = window.confirm('Pressionando "OK", os valores serão atualizados automaticamente e o formulário será enviado.\nPressionando "Cancelar", você poderá ajustar manualmente os valores. ');
            var r2 = window.confirm('Pressionando "OK", os valores serão atualizados automaticamente.\nPressionando "Cancelar", você poderá ajustar manualmente os valores. ');
            if(r2 === true){
                calculosEvolucao.ajustaValoresEvolucaoMI();
                return false;
            }else{
                return false;
            }
        }
        
        jQuery('#formulario').submit();
    });
    
    jQuery('#btn_voltar').click(function(){
        window.location.href = 'obras2.php?modulo=principal/listaEvolucaoMi&acao=A';
    });
    
    calculosEvolucao.atualizaLinhasTotais();
    
});

jQuery(function($){
        $.datepicker.regional['pt-BR'] = {
                closeText: 'Fechar',
                prevText: '&#x3c;Anterior',
                nextText: 'Pr&oacute;ximo&#x3e;',
                currentText: 'Hoje',
                monthNames: ['Janeiro','Fevereiro','Mar&ccedil;o','Abril','Maio','Junho','Julho','Agosto','Setembro','Outubro','Novembro','Dezembro'],
                monthNamesShort: ['Jan','Fev','Mar','Abr','Mai','Jun','Jul','Ago','Set','Out','Nov','Dez'],
                dayNames: ['Domingo','Segunda-feira','Ter&ccedil;a-feira','Quarta-feira','Quinta-feira','Sexta-feira','S&aacute;bado'],
                dayNamesShort: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                dayNamesMin: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                weekHeader: 'Sm',
                dateFormat: 'dd/mm/yy',
                firstDay: 0,
                isRTL: false,
                showMonthAfterYear: false,
                yearSuffix: ''};
        $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
    });
    

</script>

<style>
    .div_fotos { list-style-type: none; margin: 0; padding: 0;padding-top:3px}
    .div_fotos li { font-size: 1.2em; height: 110px; height:90px;padding: 1px}
    html>body .div_fotos li { height: 80px; line-height: 1.2em; }
    .field_fotos{padding:10px;}
    .div_fotos{height:300px;overflow:auto;}
    .draggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;cursor:move;text-align:center;background-color:#FFFFFF}
    .nodraggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
    .draggable_space{line-height: 1.2em;width:110px;height:90px;margin:3px;float:left;cursor:pointer;background-color:#CCCCCC}
    .f_selected{border: solid 1px red;}
    .fechar{position:relative;margin-left:105px;top:-8px;cursor:pointer}
    .img_foto{z-index:2;}
    .img_class{margin-top:-15px;}
</style>

<div id="divForm">
    <form name="formulario" method="post" id="formulario" action="" enctype="multipart/form-data">
        <input type="hidden" name="requisicao" id="requisicao" value="<?php echo $req;?>"/>
        <input type="hidden" name="emiid"      id="emiid"      value="<?php echo $emiid;?>"/>
        <input type="hidden" name="req"        id="req"        value="<?php echo $req;?>"/>

        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="subtitulocentro" colspan="3">Dados do Acompanhamento</td>
            </tr>
            <tr>
                <td colspan="3">
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                        <tr>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td rowspan="5"  align="right" valign="top" width="1">
                                <?php
                                    $docid = $arrDadosEvolucaoMi['docid'];
                                    if($docid){
                                        wf_desenhaBarraNavegacao($docid, array('emiid' =>  $emiid));
                                    }
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="subtitulodireita" width="190px">Data do Acompanhamento:</td>
                            <td>
                                <?php 
                                    if(empty($arrDadosEvolucaoMi['emidtevolucao'])){
                                      $data = ''; 
                                    }else{
                                      $data = date_create($arrDadosEvolucaoMi['emidtevolucao']); 
                                      $data = date_format ( $data, 'd/m/Y' );
                                    }
                                ?>
                                <input type="text" id="emidtevolucao" name="emidtevolucao" value="<?php echo $data;?>" size="15" maxlength="10" class="normal  obrigatorio "> 
                                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                            </td>
                            <td>&nbsp;</td>                
                        </tr>
                        <tr>
                            <td class="subtitulodireita" width="190px">Data Prevista de Conclusão:</td>
                            <td>
                                <?php 
                                    if(empty($arrDadosEvolucaoMi['emidtconclusaoprev'])){
                                      $data = ''; 
                                    }else{
                                      $data = date_create($arrDadosEvolucaoMi['emidtconclusaoprev']); 
                                      $data = date_format ( $data, 'd/m/Y' );
                                    }
                                ?>
                                <input type="text" id="emidtconclusaoprev" name="emidtconclusaoprev" value="<?php echo $data;?>" size="15" maxlength="10" class="normal  obrigatorio " > 
                                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                            </td>
                            <td>&nbsp;</td>                  
                        </tr>
                        <tr>
                            <td class="subtitulodireita" width="190px">Nome do Responsável:</td>
                            <td>
                                &nbsp;
                                <?php
                                    if(empty($arrDadosEvolucaoMi['usucpf'])){
                                        $cpf_vistoriador  = $_SESSION['usucpf'];
                                    }else{
                                        $cpf_vistoriador  = $arrDadosEvolucaoMi['usucpf'];
                                    }

                                    $evoMi = new EvolucaoMi();
                                    $arrUsuario = $evoMi->getDadosUsuarioCpf($cpf_vistoriador);
                                    $nome_vistoriador = $arrUsuario['usunome'];
                                ?>
                                <span id="usunome_span"><?php echo $nome_vistoriador; ?></span>
                                <input type="hidden" name="usucpf" id="usucpf" value="<?php echo $cpf_vistoriador; ?>">
                                <?php print obrigatorio(); ?>
                            </td>
                            <td>&nbsp;</td>
                        </tr>
                        <?php
                            if($_GET['acao'] == 'V'){

                                if(empty($arrDadosEvolucaoMi['emivalidador'])){
                                        $cpf_vistoriador  = $_SESSION['usucpf'];
                                }else{
                                    $cpf_vistoriador  = $arrDadosEvolucaoMi['emivalidador'];
                                }

                                $evoMi = new EvolucaoMi();
                                $arrUsuario = $evoMi->getDadosUsuarioCpf($cpf);
                                $nome_vistoriador = $arrUsuario['usunome'];

                                echo '<tr>
                                        <td class="subtitulodireita" width="190px">Nome do Validador:</td>
                                        <td>
                                            &nbsp;';
                                echo '<span id="emivalidador_span">'.$nome_vistoriador.'</span>';
                                echo '<input type="hidden" name="emivalidador" id="emivalidador" value="'.$cpf_vistoriador.'">';
                                echo obrigatorio(); 
                                echo '  </td>
                                        <td>&nbsp;</td>
                                        <!--<td>&nbsp;</td>-->
                                      </tr>';
                            }
                        ?>
                        <tr>
                            <td>&nbsp;</td> <td>&nbsp;</td> <td>&nbsp;</td> <td>&nbsp;</td>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr>
                <td colspan="3">&nbsp;</td>
            </tr>
            <?php linhasEvolucaoMi($arrDadosEvolucaoMi); ?>
            <tr>
                <td class="subtitulocentro" colspan="3">Detalhamento de Supervisão e Acompanhamento</td>
            </tr>
            <?php linhasDetalhamento($obrid, $req); ?>        
            <tr bgcolor="#c0c0c0">
                <td colspan="3" style="text-align: center"> &nbsp;</td>
            </tr>

        </table>
        
        <?php
        
        if(!empty($_SESSION['obras2']['emiid'])){
            $displayFotos = 'display: inline;';
        }else{
            $displayFotos = 'display: none;';
            echo '<center><p><input type="button" id="inserir_fotos" name="inserir_fotos" 
                                     onmouseover="return escape(\'Gerenciamento de fotos do acompanhamento (inserir, alterar e remover).\');" 
                                     value="Inserir Fotos" onclick="inserirNovasFotos();"></p></center>';
        }
        
        ?>
        
        <div id="area_fotos" style=" <?php echo $displayFotos;?>">
            <a name="divFotos"></a>
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <input type="hidden" name="hdn_fotos_galeria" id="hdn_fotos_galeria" value="" />
                    <input type="hidden" name="hdn_fotos_supervisao" id="hdn_fotos_supervisao" value="" />
                        <tr>
                            <td bgcolor="#DCDCDC" colspan="2">
                                <b>Fotos do Acompanhamento</b>
                                <?php if ($emiid): ?>
                                    *Arraste as imagens para a área de 'Fotos da Galeria' para adicioná-las à galeria. Após adicioná-las, clique no botão 'Salvar' para confirmar as alterações.
                                <?php else: ?>
                                    *Após adicionar as fotos, clique no botão 'Salvar' para confirmar as alterações.
                                <?php endif; ?>
                            </td>
                        </tr>
                        <tr>
                            <td <?php echo $emiid ? "width=\"50%\"" : "colspan='2'" ?> >
                                <fieldset class="field_fotos">
                                    <legend>Fotos do Acompanhamento</legend>
                                    <ul id="fotos_supervisao" class="div_fotos">
                <?php 
                
                if ($emiid) {
                    $sql = "SELECT fot.*, arq.arqdescricao 
                            FROM obras2.fotos AS fot
                            LEFT JOIN public.arquivo AS arq ON arq.arqid = fot.arqid
                            WHERE 
                                obrid =" . $obrid . " AND
                                emiid=" . $emiid . "
                            ORDER BY fotordem;";
                    $fotos = ($db->carregar($sql));
                }
                
                if (!$emiid) {
                        $sql = "SELECT
                                    arq.arqid
                                FROM
                                    public.arquivo arq
                                INNER JOIN
                                    obras2.arquivosobra oar ON oar.arqid = arq.arqid AND
                                                                                       oar.aqostatus = 'A'
                                INNER JOIN
                                    seguranca.usuario seg ON seg.usucpf = oar.usucpf
                                INNER JOIN
                                    obras2.evolucaomi emi ON emi.obrid = oar.obrid
                                WHERE
                                    oar.obrid = {$_SESSION['obras2']["obrid"]} AND
                                    tpaid = " . TIPO_ARQUIVO_FOTO_VISTORIA . "
                                AND
                                    (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
                                ORDER BY
                                    arq.arqid";
                    } else {
                        $sql = "SELECT DISTINCT
                                    arq.arqid
                                FROM
                                    obras2.fotos fot
                                INNER JOIN
                                    public.arquivo arq ON arq.arqid = fot.arqid
                                INNER JOIN
                                    obras2.arquivosobra oar ON arq.arqid = oar.arqid
                                WHERE
                                    fot.obrid         = {$_SESSION['obras2']["obrid"]}
                                    AND fot.emiid     = {$emiid}
                                    AND oar.tpaid     = " . TIPO_ARQUIVO_FOTO_VISTORIA . "
                                    AND oar.aqostatus = 'A'
                                ORDER BY
                                    arq.arqid";
                    }

                    $arrFotosGaleria = $db->carregar($sql);

                    // array que armazenará o arqid da Galeria
                    $arrArqidGaleria = array();
                    if (is_array($arrFotosGaleria)) {
                        foreach ($arrFotosGaleria as $foto) {
                            $arrArqidGaleria[] = $foto['arqid'];
                        }
                }
                
                if (is_array($fotos)){ 
                    $arrFotos = $fotos;
                    $n = 1; 
                    foreach ($arrFotos as $foto){ 
                            $pagina = floor($n / 16); 

                            $sql = "SELECT arqtipo, arqid  FROM public.arquivo
                                    WHERE arqid = '" . $foto['arqid'] . "'";
                            $dados = $db->pegaLinha($sql);

                            $imgend = APPRAIZ . 'arquivos/obras2/' . floor($foto['arqid'] / 1000) . '/' . $foto['arqid'];

                            if (is_file($imgend)) {
                                $img_max_dimX = 100;
                                $img_max_dimY = 85;

                                $imginfo = getimagesize($imgend);

                                $width = $imginfo[0];
                                $height = $imginfo[1];

                                if (($width > $img_max_dimX) or ($height > $img_max_dimY)) {
                                    if ($width > $height) {
                                        $w = $width * 0.9;
                                        while ($w > $img_max_dimX) {
                                            $w = $w * 0.9;
                                        }
                                        $w = round($w);
                                        $h = ($w * $height) / $width;
                                    } else {
                                        $h = $height * 0.9;
                                        while ($h > $img_max_dimY) {
                                            $h = $h * 0.9;
                                        }
                                        $h = round($h);
                                        $w = ($h * $width) / $height;
                                    }
                                } else {
                                    $w = $width;
                                    $h = $height;
                                }

                                $tamanho = " width=\"$w\" height=\"$h\" ";
                            } else {
                                $tamanho = "";
                            }
                            $eschema = ($foto['obrid_1'] ? 'obras' : 'obras2');
                            ?>
                            <li id="foto_<?php echo $foto['arqid'] ?>" class="draggable<?php echo in_array($foto['arqid'], $arrArqidGaleria) ? " f_selected" : "" ?>">
                                <img <?php echo $tamanho; ?> class="img_foto" ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>', '<?php echo $pagina ?>')" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=<?php echo $eschema ?>" />
                            </li>
                        <?php $n++;
                        }
                    }  ?>
                        </ul>
                    </fieldset>
                </td>
                <?php 
                
                    
                if ($emiid){ 
                ?>
                    <td>
                        <fieldset class="field_fotos">
                            <legend>Fotos da Galeria</legend>
                            <ul id="fotos_galeria" class="div_fotos">
                    <?php 
                        if (is_array($arrFotosGaleria)){ 
                            $n = 1;
                            foreach ($arrFotosGaleria as $foto){
                                $pagina = floor($n / 16); 
                                $sql = "SELECT arqtipo, arqid  FROM public.arquivo
                                        WHERE arqid = '" . $foto['arqid'] . "'";
                                $dados = $db->pegaLinha($sql);

                                $imgend = APPRAIZ . 'arquivos/' . (($_REQUEST["_sisarquivo"]) ? $_REQUEST["_sisarquivo"] : $_SESSION["sisarquivo"]) . '/' . floor($foto['arqid'] / 1000) . '/' . $foto['arqid'];

                                if (is_file($imgend)) {
                                    $img_max_dimX = 100;
                                    $img_max_dimY = 85;

                                    $imginfo = getimagesize($imgend);

                                    $width = $imginfo[0];
                                    $height = $imginfo[1];

                                    if (($width > $img_max_dimX) or ($height > $img_max_dimY)) {
                                        if ($width > $height) {
                                            $w = $width * 0.9;
                                            while ($w > $img_max_dimX) {
                                                $w = $w * 0.9;
                                            }
                                            $w = round($w);
                                            $h = ($w * $height) / $width;
                                        } else {
                                            $h = $height * 0.9;
                                            while ($h > $img_max_dimY) {
                                                $h = $h * 0.9;
                                            }
                                            $h = round($h);
                                            $w = ($h * $width) / $height;
                                        }
                                    } else {
                                        $w = $width;
                                        $h = $height;
                                    }

                                    $tamanho = " width=\"$w\" height=\"$h\" ";
                                } else {
                                    $tamanho = "";
                                }
                                ?>
                                <li id="s_foto_<?php echo $foto['arqid'] ?>" class="nodraggable">
                                    <img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" onclick="removerFotoGaleria('foto_<?php echo $foto['arqid'] ?>')" />
                                    <img <?php echo $tamanho; ?> class="img_class" ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>', '<?php echo $pagina ?>')" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85" />
                                </li>
                            <?php $n++;
                            }
                        } ?>
                    </ul>
                </fieldset>
                </td>
                <?php } ?>
                </tr>
                <tr>
                    <td bgcolor="#c0c0c0" colspan="2" align="center">
                        <input <?php echo (($habilitadoFoto) ? "" : "disabled=\"disabled\""); ?> 
                               type="button" 
                               id="add_fotos" 
                               name="add_fotos" 
                               onmouseover="return escape('Gerenciamento de fotos do acompanhamento (inserir, alterar e remover).');" 
                               value="<?php echo ($emiid ? 'Alterar Fotos' : 'Inserir Fotos') ?>" 
                               onclick="ImageComponent( <?php if (isset($emiid)){ echo"'?funcao=AtualizaFotos&emiid=".$emiid."'";}else{ echo "' '";}?>);">
                        <?php
                        if ($emiid):
                        ?>
                            <input <?php echo (($habilitadoFoto) ? "" : "disabled=\"disabled\""); ?> 
                                   type="button" 
                                   id="btn_salvar_fotos" 
                                   name="btn_salvar_fotos" 
                                   onmouseover="return escape('Salvar as alterações das fotos de acompanhamento e galeria.');" 
                                   value="Salvar Fotos" 
                                   onclick="salvarFotos()">
                        <?php
                        endif;
                        ?>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                    </td>
                </tr>
            </table>
        </div>
        
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">        
            <tr bgcolor="#c0c0c0">
                <td colspan="3" style="text-align: center">
                    <!--<input type="button" value="Salvar" id="btn_salvar_evolucao" style="cursor:pointer;"/>-->
                    <?php 
                        echo $btn_salvar_form;
                        echo $btn_cancela_form;
                    ?>
                </td>
            </tr>
            <tr bgcolor="#c0c0c0">
                <td colspan="3" style="text-align: center"> &nbsp;</td>
            </tr>        
        </table>
    
    </form>
</div>

<script lang="javascript">
            
setTimeout(function(){
    
    jQuery('#emidtevolucao').mask('99/99/9999');
    jQuery('#emidtconclusaoprev').mask('99/99/9999');
    
    var d = new Date();
    var data_hoje = d.toUTCString();
    
    jQuery("#emidtevolucao").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        maxDate: new Date(data_hoje),
        showAnim:'drop'
    });
    jQuery("#emidtconclusaoprev").datepicker({
        minDate: 0,
        numberOfMonths: 1,
        showWeek: true
        , onClose: function(selectedDate) {
            $("#emidtevolucao").datepicker("option", "minDate", selectedDate);
        }
        , 'showAnim':'drop'
    });

    <?php
    $arPflcod = array(PFLCOD_SUPERVISOR_MEC,
        PFLCOD_ADMINISTRADOR,
        PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
        PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
        PFLCOD_SUPERVISOR_UNIDADE,
        PFLCOD_GESTOR_UNIDADE,
        PFLCOD_GESTOR_MEC);
    if (possuiPerfil($arPflcod)) {
        ?>
            $(function() {
                $("#fotos_supervisao").sortable({
                    placeholder: "draggable_space",
                    connectWith: "#fotos_supervisao",
                    cancel: ".nodraggable"
                });
                $("#fotos_galeria").droppable({
                drop: function(event, ui) {
                if (!$("#s_" + ui.draggable.attr("id")).html()){
                $("<li id=\"s_" + ui.draggable.attr("id") + "\" class=\"nodraggable\" ><img src=\"../imagens/fechar.jpeg\" title=\"Remover Foto\" class=\"fechar\" onclick=\"removerFotoGaleria('" + ui.draggable.attr("id") + "')\" />" + ui.draggable.html() + "</li>").appendTo(this);
                        $("#s_" + ui.draggable.attr("id") + " .img_foto").attr("style", "margin-top:-15px");
                        $("#" + ui.draggable.attr("id")).attr("class", "draggable f_selected");
                }
                }
                }).sortable({
                cancel: ".nodraggable"
                });
            });
                            
    <?php } ?>
               
}, 500);

</script>
                                        
<?php

/**
 * 
 * @param array $arrDadosEvolucaoMi
 */
function linhasEvolucaoMi($arrDadosEvolucaoMi){
    
    $arrDadosEvolucaoMi['emipecentmedidoedif']   = number_format($arrDadosEvolucaoMi['emipecentmedidoedif'], 2, ',','.');
    $arrDadosEvolucaoMi['emipecentmedidoext']    = number_format($arrDadosEvolucaoMi['emipecentmedidoext'], 2, ',','.');
    $arrDadosEvolucaoMi['emivlmedidoedif']       = number_format($arrDadosEvolucaoMi['emivlmedidoedif'], 2, ',','.');
    $arrDadosEvolucaoMi['emivlmedidoext']        = number_format($arrDadosEvolucaoMi['emivlmedidoext'], 2, ',','.');
    $arrDadosEvolucaoMi['emipecentvalidadoedif'] = number_format($arrDadosEvolucaoMi['emipecentvalidadoedif'], 2, ',', '.');
    $arrDadosEvolucaoMi['emipecentvalidadoext']  = number_format($arrDadosEvolucaoMi['emipecentvalidadoext'], 2, ',', '.');
    $arrDadosEvolucaoMi['emivlvalidadoedif']     = number_format($arrDadosEvolucaoMi['emivlvalidadoedif'], 2, ',', '.');
    $arrDadosEvolucaoMi['emivlvalidadoext']      = number_format($arrDadosEvolucaoMi['emivlvalidadoext'], 2, ',', '.');

    if($_GET['acao'] != 'V'){
        echo 
        '<tr>
            <td colspan="3">
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <th class="subtitulodireita" width="140px">&nbsp;</th>
                        <th colspan="2" style="text-align: center"> Aferido </th>
                    </tr>
                    <tr>
                        <th class="subtitulodireita" width="140px">&nbsp;</th>
                        <th>Edificação</th>
                        <th>Serviços Externos</th>
                    </tr>
                    <tr>
                        <td class="subtitulodireita" width="140px">% da Evolução</td>
                        <td>'.campo_texto('emipecentmedidoedif', 'S', false, '', 17, 15, '###,##', '', 'left', '', 0, 'id="emipecentmedidoedif"', '', $arrDadosEvolucaoMi['emipecentmedidoedif'], '').'</td>
                        <td>'.campo_texto('emipecentmedidoext', 'S', false, '', 17, 15, '###,##', '', 'left', '', 0, 'id="emipecentmedidoext"', '', $arrDadosEvolucaoMi['emipecentmedidoext'], '').'</td>
                    </tr>
                    <tr>
                        <td class="subtitulodireita" width="140px">Valor da Evolução (R$)</td>
                        <td>'.campo_texto('emivlmedidoedif', 'S', false, '', 17, 15, '###.###.###.###,##', '', 'left', '', 0, 'id="emivlmedidoedif"', '', $arrDadosEvolucaoMi['emivlmedidoedif'], '').'</td>
                        <td>'.campo_texto('emivlmedidoext', 'S', false, '', 17, 15, '###.###.###.###,##', '', 'left', '', 0, 'id="emivlmedidoext"', '', $arrDadosEvolucaoMi['emivlmedidoext'], '').'</td>
                    </tr>
                </table>
            </td>
        </tr>';
    }
    else{
        echo 
        '<tr>
            <td colspan="3">
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <th class="subtitulodireita" width="140px">&nbsp;</th>
                        <th colspan="2" style="text-align: center"> Aferido </th>
                        <th colspan="2" style="text-align: center"> Validado </th>
                    </tr>
                    <tr>
                        <th class="subtitulodireita" width="140px">&nbsp;</th>
                        <th>Edificação</th>
                        <th>Serviços Externos</th>
                        <th>Edificação</th>
                        <th>Serviços Externos</th>
                    </tr>
                    <tr>
                        <td class="subtitulodireita" width="140px">% da Evolução</td>
                        <td>'.($arrDadosEvolucaoMi['emipecentmedidoedif']).'</td>
                        <td>'.($arrDadosEvolucaoMi['emipecentmedidoext']).'</td>
                        <td>'.campo_texto('emipecentvalidadoedif', 'S', false, '', 17, 15, '###,##', '', 'left', '', 0, 'id="emipecentvalidadoedif"', '', $arrDadosEvolucaoMi['emipecentvalidadoedif'],'').'</td>
                        <td>'.campo_texto('emipecentvalidadoext', 'S', false, '', 17, 15, '###,##', '', 'left', '', 0, 'id="emipecentvalidadoext"', '', $arrDadosEvolucaoMi['emipecentvalidadoext'],'').'</td>
                    </tr>
                    <tr>
                        <td class="subtitulodireita" width="140px">Valor da Evolução (R$)</td>                            
                        <td>'.($arrDadosEvolucaoMi['emivlmedidoedif']).'</td>
                        <td>'.($arrDadosEvolucaoMi['emivlmedidoext']).'</td>
                        <td>'.campo_texto('emivlvalidadoedif', 'S', false, '', 17, 15, '###.###.###.###,##', '', 'left', '', 0, 'id="emivlvalidadoedif"', '', $arrDadosEvolucaoMi['emivlvalidadoedif'],'').'</td>
                        <td>'.campo_texto('emivlvalidadoext', 'S', false, '', 17, 15, '###.###.###.###,##', '', 'left', '', 0, 'id="emivlvalidadoext"', '', $arrDadosEvolucaoMi['emivlvalidadoext'],'').'</td>
                    </tr>
                </table>
            </td>
        </tr>';
    }
}
/**
 * 
 * @param int    $obrid
 * @param string $tipo
 */
function linhasDetalhamento($obrid, $tipo = 'cadastro'){
    $emiid = $_SESSION['obras2']['emiid'];
    $evmi= new EvolucaoMi();
    switch ($tipo) {
        case 'cadastro':
            $_SESSION['obras2']['evoMiStatus'] = 'cadastro';
            $id_ultima_evolucao = $evmi->verificaUltimaEvolucaoMi($obrid);
            //Verificação de diferenças entre a quantidade de itens da última evolução, com os itens do cronograma.
            //Se houver diferença(s), "recarregar" a lista de itens da evolução com os itens do cronograma com os dados calculados da última evolução.
            if($id_ultima_evolucao){
                $possui_diferenças = $evmi->verificaDiferencasUltimaEvolucaoMiVsCronograma($id_ultima_evolucao, $obrid);
            }
            
            $somenteLeitura = false;
            if($possui_diferenças){
                $difItens  = false;
                $difServex = false;
                foreach ($possui_diferenças as $val) {
                    if($val['tipo'] == 'itens'){
                        $difItens  = true;
                        //Busca os dados Calculados da última evolução !
                        $arItens_calc = $evmi->getItensEvolucaoMiByEmiidObrid($id_ultima_evolucao, $obrid);
                        $arItens      = $evmi->getItemComposicaoObraByObra($obrid);
                        foreach ($arItens as $key => &$value) {
                            foreach ($arItens_calc as $k => $v) {
                                if($v['icoid'] == $value['icoid'] ){
                                    $value['dadosCalculados'] = $v['dadosCalculados'];
                                }
                            }
                        }
                    }elseif($val['tipo'] == 'servex'){
                        $difServex = true;
                        //Busca os dados Calculados da última evolução !
                        $arItensServExterno_calc = $evmi->getItensEvolucaoMiByEmiidObrid($id_ultima_evolucao, $obrid , 'F');
                        $arItensServExterno      = $evmi->getItemComposicaoObraByObra($obrid, 'F');    
                        foreach ($arItensServExterno as $key => &$value) {
                            foreach ($arItensServExterno_calc as $k => $v) {
                                if($v['icoid'] == $value['icoid'] ){
                                    $value['dadosCalculados'] = $v['dadosCalculados'];
                                }
                            }
                        }
                    }
                }
                
                if(!$difItens) { $arItens            = $evmi->getItensEvolucaoMiByEmiidObrid($id_ultima_evolucao, $obrid); }
                if(!$difServex){ $arItensServExterno = $evmi->getItensEvolucaoMiByEmiidObrid($id_ultima_evolucao, $obrid , 'F'); }
                
                $html_linhas_d      = getHtmlLinhasDetalhamento($arItens,            'D', $somenteLeitura, 'cronoAtu');
                $html_linhas_f      = getHtmlLinhasDetalhamento($arItensServExterno, 'F', $somenteLeitura, 'cronoAtu');
            }else{
                if($id_ultima_evolucao){
                    //Quando existe evolução cadastrada, pega os dados da última evolução
                    $arItens            = $evmi->getItensEvolucaoMiByEmiidObrid($id_ultima_evolucao, $obrid);
                    $arItensServExterno = $evmi->getItensEvolucaoMiByEmiidObrid($id_ultima_evolucao, $obrid , 'F');
                    if(empty($arItens)){
                        $html_linhas_d = getHtmlLinhasDetalhamento($arItens, 'D', $somenteLeitura, 'crono');
                    }                
                    if(empty($arItensServExterno)){
                        $html_linhas_f = getHtmlLinhasDetalhamento($arItensServExterno, 'F', $somenteLeitura, 'crono');
                    }                
                    $html_linhas_d = getHtmlLinhasDetalhamento($arItens,            'D', $somenteLeitura, 'evo');
                    $html_linhas_f = getHtmlLinhasDetalhamento($arItensServExterno, 'F', $somenteLeitura, 'evo');                
                }else{
                    //Quando não existe nenhuma evolução cadastrada, pega "novos" dados do cronograma para gerar a evolução
                    //Ou quando o número de itens ou seus valores do cronograma foram alterados
                    $arItens            = $evmi->getItemComposicaoObraByObra($obrid);            
                    $arItensServExterno = $evmi->getItemComposicaoObraByObra($obrid, 'F');                
                    $html_linhas_d      = getHtmlLinhasDetalhamento($arItens,            'D', $somenteLeitura, 'crono');
                    $html_linhas_f      = getHtmlLinhasDetalhamento($arItensServExterno, 'F', $somenteLeitura, 'crono');                
                }
            }
            
            
            imprimeTabelaDetalhamento($html_linhas_d, $html_linhas_f, 'cad/ed');            
            break;

        case 'edicao':
            $emiid = $_SESSION['obras2']['emiid'];
            $_SESSION['obras2']['evoMiStatus'] = 'edicao';
            $somenteLeitura = false;            
            //Quando existe evolução cadastrada, pega os dados da última evolução
            $arItens            = $evmi->getItensEvolucaoMiByEmiidObrid($emiid, $obrid);
            $arItensServExterno = $evmi->getItensEvolucaoMiByEmiidObrid($emiid, $obrid , 'F');
            $html_linhas_d = getHtmlLinhasDetalhamento($arItens,            'D', $somenteLeitura, 'evoEdt');
            $html_linhas_f = getHtmlLinhasDetalhamento($arItensServExterno, 'F', $somenteLeitura, 'evoEdt');            
            imprimeTabelaDetalhamento($html_linhas_d, $html_linhas_f, 'cad/ed');            
            break;
        
        case 'validacao':
            $_SESSION['obras2']['evoMiStatus'] = 'validacao';
            $emiid = $_SESSION['obras2']['emiid'];
            $somenteLeitura = false;            
            //Quando existe evolução cadastrada, pega os dados da última evolução
            $arItens            = $evmi->getItensEvolucaoMiByEmiidObrid($emiid, $obrid);
            $arItensServExterno = $evmi->getItensEvolucaoMiByEmiidObrid($emiid, $obrid , 'F');
            $html_linhas_d = getHtmlLinhasDetalhamento($arItens,            'D', $somenteLeitura, 'validacao');
            $html_linhas_f = getHtmlLinhasDetalhamento($arItensServExterno, 'F', $somenteLeitura, 'validacao');            
            imprimeTabelaDetalhamento($html_linhas_d, $html_linhas_f, 'valida/ed');            
            break;
        default:
            break;
    }
}
/**
 * 
 * @param array   $arr_dados
 * @param char(1) $tipo_dado
 * @param bool    $somenteLeitura
 * @param string  $tipo_linha
 * @return string
 */
function getHtmlLinhasDetalhamento($arr_dados = array(), $tipo_dado = 'D', $somenteLeitura = false, $tipo_linha = 'crono'){
    $html_linhas = '';
    
    if($tipo_dado == 'F'){
        $tipo_item = 'serviço externo';
    }else{
        $tipo_item = 'item'; //Item cronograma
    }
    
    $html_linha_vazia = ' <tr>
                                <td colspan="18" style="text-align: center"> &nbsp; </td>
                             </tr> ';
    $percent_item_obra = 0;
    
    $counter = 0;
    $str_bg  = '';
    foreach ($arr_dados as $key => $value) {
        $counter++;
        $str_bg = ($counter % 2 == '') ? '' :' bgcolor="#FFFFFF" ';
        $value['iemiid']  = (isset($value['iemiid']) && !empty($value['iemiid'])) ? $value['iemiid'] : 0;
        $value['umdeesc'] = (!empty($value['umdeesc'])) ? $value['umdeesc'] : '-';
        
        $dadosCalculados = $value['dadosCalculados'];
        
        //Monta as linhas de Edificação
        if($tipo_dado == 'D'){
            $html_linhas .= '<tr id="ln_icoid_d_'.$value['icoid'].'" '.$str_bg.'>';
            //Descrição
            $html_linhas .= '<td colspan="3"> 
                                <input type="hidden" name="icoid_d[]" value="'.$value['icoid'].'">
                                <input type="hidden" name="iemiid_d[]" value="'.$value['iemiid'].'">
                                <img src="/imagens/seta_filho.gif" border="0">
                                '.$value['itcdesc'].'
                             </td>';
            //Valor R$
            $html_linhas .= '<td> '.number_format($value['icovlritem'], 2, ',', '.').'</td>';
            //% Sobre a Obra
            $html_linhas .= '<td> '.number_format($value['icopercsobreobra'], 2, ',', '.').'</td>';
            //Quantidade
            $html_linhas .= '<td> - </td>';
            //Unidade de Medida
            $html_linhas .= '<td> - </td>';
            //Data de Início
            $html_linhas .= '<td> '.formata_data($value['icodtinicioitem']).'</td>';
            //Data de Término
            $html_linhas .= '<td> '.formata_data($value['icodterminoitem']).'</td>';
            
            $linha_totais = ' 
                                    <tr id="linha_totais_d">
                                      <th colspan="3"> TOTAL: </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                    </tr>
                                ';                
            $linha_totais .= $html_linha_vazia;
            
            switch ($tipo_linha) {
                // Quando não possui evolução, os dados vem do cronograma
                case 'crono':                    
                    //************Execução Acumulada
                    //(%) do Item já executado
                    $html_linhas .= '<td> '.number_format(0, 2, ',', '.').' </td>';
                    //(%) do Item já executado sobre a Obra
                    $html_linhas .= '<td> '.number_format(0, 2, ',', '.').' </td>';                    
                    //************Saldo a executar
                    //(%) Supervisão
                    $html_linhas .= '<td> '.number_format(100, 2, ',', '.').' </td>';
                    //Valor 
                    $html_linhas .= '<td> '.number_format($value['icovlritem'], 2, ',', '.').' </td>';                    
                    //************Supervisão Atual
                    //(%) do Item já executado sobre a Obra após Supervisão
                    $html_linhas .= '<td> '.number_format(0, 2, ',', '.').' </td>';
                    //Quantidade Executada
                    $html_linhas .= '<td> - </td>';
                    //(%) executado
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemipecentmedido[]', 'N', $somenteLeitura, '', 7, 6, '###,##', '', 'right', '', 0, '', 
                                        "calculosEvolucao.keyupAction(this, 'D', '%');", 
                                        number_format(0, 2, ',', '.')).' 
                                     </td>';
                    //Valor Executado
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemivlmedido[]', 'N', $somenteLeitura, '', 7, 18, '###.###.###.###,##', '', 'right', '', 0, '', 
                                        "calculosEvolucao.keyupAction(this, 'D','vl');", 
                                        number_format(0, 2, ',', '.')).' 
                                     </td>';

                    $html_linhas .= '</tr>';
                    break;
                // Quando os dados calculados vem da última evolução, mas os itens vem do cronograma
                case 'cronoAtu':
                    //************Aplicar Calculos************//
                    //************Execução Acumulada
                    //(%) do Item já executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumPercValid'], 2, ',', '.').' </td>';
                    //(%) do Item já executado sobre a Obra -- 
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sobreObraPerc'], 2, ',', '.').' </td>';
                    //************Saldo a executar
                    //(%) Supervisão
//                    $html_linhas .= '<td> '.number_format(12, 2, ',', '.').' </td>';
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoPerc'], 2, ',', '.').' </td>';
                    //Valor Executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoVl'], 2, ',', '.').' </td>';
                    //************Supervisão Atual
                    //(%) do Item já executado sobre a Obra após Supervisão
                    $html_linhas .= '<td> '.number_format(0, 2, ',', '.').' </td>';
                    //Quantidade Executada
                    $html_linhas .= '<td> - </td>';
                    //(%) executado
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemipecentmedido[]', 'N', $somenteLeitura, '', 7, 6, '###,##', '', 'right', '',0, '', 
                                        "calculosEvolucao.keyupAction(this, 'D', '%');",
                                        number_format(0, 2, ',', '.')).'
                                     </td>';
                    //Valor Executado
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemivlmedido[]', 'N', $somenteLeitura, '', 7, 18, '###.###.###.###,##', '', 'right', '', 0, '', 
                                        "calculosEvolucao.keyupAction(this, 'D','vl');", 
                                        number_format(0, 2, ',', '.')).'
                                     </td>';

                    $html_linhas .= '</tr>';
                    break;
                // Quando os dados vem da última evolução
                case 'evo':
                    //************Aplicar Calculos************//
                    //************Execução Acumulada
                    //(%) do Item já executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumPercValid'], 2, ',', '.').' </td>';
                    //(%) do Item já executado sobre a Obra -- 
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sobreObraPerc'], 2, ',', '.').' </td>';
                    //************Saldo a executar
                    //(%) Supervisão
//                    $html_linhas .= '<td> '.number_format(12, 2, ',', '.').' </td>';
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoPerc'], 2, ',', '.').' </td>';
                    //Valor Executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoVl'], 2, ',', '.').' </td>';
                    //************Supervisão Atual
                    //(%) do Item já executado sobre a Obra após Supervisão
                    $html_linhas .= '<td> '.number_format(0, 2, ',', '.').' </td>';
                    //Quantidade Executada
                    $html_linhas .= '<td> - </td>';
                    //(%) executado
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemipecentmedido[]', 'N', $somenteLeitura, '', 7, 6, '###,##', '', 'right', '',0, '', 
                                        "calculosEvolucao.keyupAction(this, 'D', '%');",
                                        number_format(0, 2, ',', '.')).'
                                     </td>';
                    //Valor Executado
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemivlmedido[]', 'N', $somenteLeitura, '', 7, 18, '###.###.###.###,##', '', 'right', '', 0, '', 
                                        "calculosEvolucao.keyupAction(this, 'D','vl');", 
                                        number_format(0, 2, ',', '.')).'
                                     </td>';

                    $html_linhas .= '</tr>';
                    break;
                // Quando os dados vem da evolução em edição
                case 'evoEdt':
                    //************Aplicar Calculos************//
                    //************Execução Acumulada
                    //(%) do Item já executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumPercValid'], 2, ',', '.').' </td>';
                    //(%) do Item já executado sobre a Obra -- 
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sobreObraPerc'], 2, ',', '.').' </td>';
                    //************Saldo a executar
                    //(%) Supervisão
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoPerc'], 2, ',', '.').' </td>';
                    //Valor Executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoVl'], 2, ',', '.').' </td>';
                    //************Supervisão Atual
                    //(%) do Item já executado sobre a Obra após Supervisão
                    $per_sobre_obra_pos_exec = ($value['iemipecentmedido'] !=0 ) ? $value['itempercsobreobra'] + $dadosCalculados['sobreObraPerc'] : 0;
                    $html_linhas .= '<td> '.number_format($per_sobre_obra_pos_exec, 2, ',', '.').' </td>';
                    //Quantidade Executada
                    $html_linhas .= '<td> - </td>';
                    //(%) executado
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemipecentmedido[]', 'N', $somenteLeitura, '', 7, 6, '###,##', '', 'right', '',0, '', 
                                        "calculosEvolucao.keyupAction(this, 'D', '%');",
                                        number_format($value['iemipecentmedido'], 2, ',', '.')).
                                     '</td>';
                    //Valor Executado
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemivlmedido[]', 'N', $somenteLeitura, '', 7, 18, '###.###.###.###,##', '', 'right', '', 0, '', 
                                        "calculosEvolucao.keyupAction(this, 'D','vl');", 
                                        number_format($value['iemivlmedido'], 2, ',', '.')).
                                     '</td>';

                    $html_linhas .= '</tr>';
                    break;
                // Quando os dados são para validação
                case 'validacao':
                    //************Aplicar Calculos************//
                    //************Execução Acumulada
                    //(%) do Item já executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumPercValid'], 2, ',', '.').' </td>';
                    //(%) do Item já executado sobre a Obra 
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sobreObraPerc'], 2, ',', '.').' </td>';
                    //************Saldo a executar
                    //(%) Supervisão
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoPerc'], 2, ',', '.').' </td>';
                    //Valor Executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoVl'], 2, ',', '.').' </td>';              
                    //************Supervisão Atual
                    //(%) do Item já executado sobre a Obra após Supervisão
                    $per_sobre_obra_pos_exec = ($value['iemipecentmedido'] !=0 ) ? $value['itempercsobreobra'] + $dadosCalculados['sobreObraPerc'] : 0;
                    $html_linhas .= '<td> '.number_format($per_sobre_obra_pos_exec, 2, ',', '.').' </td>';
//                    $html_linhas .= '<td> '.number_format($value['itempercsobreobra'], 2, ',', '.').' </td>';
                    //Quantidade Executada
                    $html_linhas .= '<td> - </td>';
                    //(%) executado
                    $html_linhas .= '<td> 
                                        '.number_format($value['iemipecentmedido'], 2, ',', '.').'
                                     </td>';
                    //Valor Executado
                    $html_linhas .= '<td> 
                                        '.number_format($value['iemivlmedido'], 2, ',', '.').'
                                     </td>';                    
                    //************Campos da Validação
                    //(%) do Item já executado sobre a Obra após Supervisão - Somado com Execução Acumulada
                    $percent_val = (!empty($value['iemipecentvalidado']) && $value['iemipecentvalidado'] != 0.00) ? $value['iemipecentvalidado'] : $value['iemipecentmedido'];
                    $percItemExecObraPosEvo = ($percent_val != 0) ? $value['itempercsobreobra'] + $dadosCalculados['sobreObraPerc'] : 0;
                    $html_linhas .= '<td> '.number_format($percItemExecObraPosEvo, 2, ',', '.').' </td>';
                    //Quantidade Executada
                    $html_linhas .= '<td> - </td>';
                    //(%) executado
                    $percent_val = (!empty($value['iemipecentvalidado']) && $value['iemipecentvalidado'] != 0.00) ? $value['iemipecentvalidado'] : $value['iemipecentmedido'];
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemipecentmedido_val[]', 'N', $somenteLeitura, '', 7, 6, '###,##', '', 'right', '',0, '', 
                                        "calculosEvolucao.keyupAction(this, 'DV', '%');", 
                                        number_format($percent_val, 2, ',', '.')).'
                                     </td>';
                    //Valor Executado
                    $vl_val = (!empty($value['iemivlvalidado']) && $value['iemivlvalidado'] != 0.00) ? $value['iemivlvalidado'] : $value['iemivlmedido'];
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemivlmedido_val[]', 'N', $somenteLeitura, '', 7, 18, '###.###.###.###,##', '', 'right', '', 0, '', 
                                        "calculosEvolucao.keyupAction(this, 'DV', 'vl');", 
                                        number_format($vl_val, 2, ',', '.')).'
                                     </td>';
                    $html_linhas .= '</tr>';
                    
                    $linha_totais = ' 
                                    <tr id="linha_totais_d_v">
                                      <th colspan="3"> TOTAL: </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                    </tr>
                                ';
                $linha_totais .= $html_linha_vazia;
                    
                    break;
                default:
                    $html_linhas = '';
                    break;
            }
        }
        //Monta as linhas de Serviços Externos
        elseif($tipo_dado == 'F'){
            
            $html_linhas .= '<tr id="ln_icoid_f_'.$value['icoid'].'" '.$str_bg.'>';
            //Descrição
            $html_linhas .= '<td> 
                                <input type="hidden" name="icoid_f[]"  value="'.$value['icoid'].'">
                                <input type="hidden" name="iemiid_f[]" value="'.$value['iemiid'].'">
                                <img src="/imagens/seta_filho.gif" border="0">
                                '.$value['itcdesc'].'
                             </td>';
            //Valor Total R$
            $html_linhas .= '<td> '.number_format($value['icovlritem_total'], 2, ',', '.').'</td>';
            //Valor Unitário R$
            $html_linhas .= '<td> '.number_format($value['icovlritem'], 2, ',', '.').'</td>';
            //% Sobre a Obra
            $html_linhas .= '<td> '.number_format($value['icopercsobreobra'], 2, ',', '.').'</td>';
            //Quantidade
            $html_linhas .= '<td> 
                                <input type="hidden" name="itcquantidade[]" value="'.$value['itcquantidade'].'">
                                '.number_format($value['itcquantidade'], 2, ',', '.').'
                             </td>';
            //Unidade de Medida
            $html_linhas .= '<td> '.$value['umdeesc'].'</td>';
            //Data de Início
            $html_linhas .= '<td> '.formata_data($value['icodtinicioitem']).'</td>';
            //Data de Término
            $html_linhas .= '<td> '.formata_data($value['icodterminoitem']).'</td>';
            
            $linha_totais = ' 
                                    <tr id="linha_totais_f">
                                      <th> TOTAL: </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>                                      
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                    </tr>
                                ';
            
            switch ($tipo_linha) {
                // Quando não possui evolução, os dados vem do cronograma
                case 'crono':
                    //************Execução Acumulada
                    //(%) do Item já executado
                    $html_linhas .= '<td> '.number_format(0, 2, ',', '.').' </td>';
                    //(%) do Item já executado sobre a Obra
                    $html_linhas .= '<td> '.number_format(0, 2, ',', '.').' </td>';
                    //Quantidade já Executada
                    $html_linhas .= '<td> '.number_format(0, 2, ',', '.').' </td>';
                    //************Saldo a executar
                    //(%) Supervisão
                    $html_linhas .= '<td> '.number_format(100, 2, ',', '.').' </td>';
                    //Valor 
                    $html_linhas .= '<td> '.number_format($value['icovlritem_total'], 2, ',', '.').' </td>';
                    //************Supervisão atual
                    //(%) Supervisão
                    $html_linhas .= '<td> '.
                                          '<span name="iemipecentmedido_f_s[]">'.number_format(0, 2, ',', '.').'</span>'.                                       
                                          '<input type="hidden" name="iemipecentmedido_f[]" value="'.number_format(0, 2, ',', '.').'">'.
                                    '</td>';
                    //Quantidade já Executada
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemiqtdmedido[]', 'N', $somenteLeitura, '', 7, 18, '###.###.###.###,##', '', 'right', '', 0, '', 
                                        "calculosEvolucao.keyupAction(this, 'F', 'qtd');",
                                        number_format(0, 2, ',', '.')).' 
                                     </td>';
                    //Valor Executado
                    $html_linhas .= '<td> '.
                                           '<span name="iemivlmedido_f_s[]">'.number_format(0, 2, ',', '.').'</span>'.
                                           '<input type="hidden" name="iemivlmedido_f[]" value="'.number_format(0, 2, ',', '.').'">'.
                                    '</td>';
                    //(%) do Item já executado sobre a Obra após Supervisão
                    $html_linhas .= '<td> '
                                            .number_format(0, 2, ',', '.').
                                    '</td>';
                    $html_linhas .= '</tr>';
                    break;
                // Quando os dados calculados vem da última evolução, mas os itens vem do cronograma
                case 'cronoAtu':
                    //************Execução Acumulada
                    //(%) do Item já executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumPercValid'], 2, ',', '.').' </td>';
                    //(%) do Item já executado sobre a Obra 
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sobreObraPerc'], 2, ',', '.').' </td>';
                    //Quantidade já Executada
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumQtdValid'], 2, ',', '.').' </td>';                    
                    //************Saldo a executar
                    //(%) Supervisão
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoPerc'], 2, ',', '.').' </td>';
                    //Valor Executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoVl'], 2, ',', '.').' </td>';                    
                    //************ Supervisão Atual
                    //(%) Supervisão
                    $html_linhas .= '<td> '.
                                          '<span name="iemipecentmedido_f_s[]">'.number_format(0, 2, ',', '.').'</span>'.                                       
                                          '<input type="hidden" name="iemipecentmedido_f[]" value="'.number_format(0, 2, ',', '.').'">'.
                                    '</td>';
                    //Quantidade já Executada
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemiqtdmedido[]', 'N', $somenteLeitura, '', 7, 18, '###.###.###.###,##', '', 'right', '', 0, '', 
                                        "calculosEvolucao.keyupAction(this, 'F', 'qtd');",
                                        number_format(0, 2, ',', '.')).' 
                                     </td>';
                    //Valor Executado 
                    $html_linhas .= '<td> '.
                                        '<span name="iemivlmedido_f_s[]">'.number_format(0, 2, ',', '.').'</span>'.
                                        '<input type="hidden" name="iemivlmedido_f[]" value="'.number_format(0, 2, ',', '.').'">'.
                                    '</td>';
                    //(%) do Item já executado sobre a Obra após Supervisão 
                    $html_linhas .= '<td colspan="2"> '.number_format(0, 2, ',', '.').' </td>';
                    $html_linhas .= '</tr>';
                    break;
                // Quando os dados vem da última evolução
                case 'evo':
                    //************Execução Acumulada
                    //(%) do Item já executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumPercValid'], 2, ',', '.').' </td>';
                    //(%) do Item já executado sobre a Obra 
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sobreObraPerc'], 2, ',', '.').' </td>';
                    //Quantidade já Executada
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumQtdValid'], 2, ',', '.').' </td>';                    
                    //************Saldo a executar
                    //(%) Supervisão
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoPerc'], 2, ',', '.').' </td>';
                    //Valor Executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoVl'], 2, ',', '.').' </td>';                    
                    //************ Supervisão Atual
                    //(%) Supervisão
                    $html_linhas .= '<td> '.
                                          '<span name="iemipecentmedido_f_s[]">'.number_format(0, 2, ',', '.').'</span>'.                                       
                                          '<input type="hidden" name="iemipecentmedido_f[]" value="'.number_format(0, 2, ',', '.').'">'.
                                    '</td>';
                    //Quantidade já Executada
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemiqtdmedido[]', 'N', $somenteLeitura, '', 7, 18, '###.###.###.###,##', '', 'right', '', 0, '', 
                                        "calculosEvolucao.keyupAction(this, 'F', 'qtd');",
                                        number_format(0, 2, ',', '.')).' 
                                     </td>';
                    //Valor Executado 
                    $html_linhas .= '<td> '.
                                        '<span name="iemivlmedido_f_s[]">'.number_format(0, 2, ',', '.').'</span>'.
                                        '<input type="hidden" name="iemivlmedido_f[]" value="'.number_format(0, 2, ',', '.').'">'.
                                    '</td>';
                    //(%) do Item já executado sobre a Obra após Supervisão 
                    $html_linhas .= '<td colspan="2"> '.number_format(0, 2, ',', '.').' </td>';
                    $html_linhas .= '</tr>';
                    break;
                // Quando os dados vem da evolução em edição
                case 'evoEdt':
                    //************Execução Acumulada
                    //(%) do Item já executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumPercValid'], 2, ',', '.').' </td>';
                    //(%) do Item já executado sobre a Obra 
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sobreObraPerc'], 2, ',', '.').' </td>';
                    //Quantidade já Executada
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumQtdValid'], 2, ',', '.').' </td>';                    
                    //************Saldo a executar
                    //(%) Supervisão
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoPerc'], 2, ',', '.').' </td>';
                    //Valor Executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoVl'], 2, ',', '.').' </td>';
                    //************Supervisão Atual
                    //(%) Supervisão
                    $html_linhas .= '<td> '.
                                          '<span name="iemipecentmedido_f_s[]">'.number_format($value['iemipecentmedido'], 2, ',', '.').'</span>'.                                       
                                          '<input type="hidden" name="iemipecentmedido_f[]" value="'.number_format($value['iemipecentmedido'], 2, ',', '.').'">'.
                                    '</td>';
                    //Quantidade já Executada
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemiqtdmedido[]', 'N', $somenteLeitura, '', 7, 18, '###.###.###.###,##', '', 'right', '', 0, '', 
                                        "calculosEvolucao.keyupAction(this, 'F', 'qtd');",
                                        number_format($value['iemiqtdmedido'], 2, ',', '.')).
                                     '</td>';
                    //Valor Executado 
                    $html_linhas .= '<td> '.
                                        '<span name="iemivlmedido_f_s[]">'.number_format($value['iemivlmedido'], 2, ',', '.').'</span>'.
                                        '<input type="hidden" name="iemivlmedido_f[]" value="'.number_format($value['iemivlmedido'], 2, ',', '.').'">'.
                                    '</td>';
                    //(%) do Item já executado sobre a Obra após Supervisão 
                    $html_linhas .= '<td colspan="2"> '.number_format($value['itempercsobreobra'], 2, ',', '.').' </td>';
                    $html_linhas .= '</tr>';
                    break;
                // Quando os dados são para validação
                case 'validacao':
                    //************Execução Acumulada
                    //(%) do Item já executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumPercValid'], 2, ',', '.').' </td>';
                    //(%) do Item já executado sobre a Obra 
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sobreObraPerc'], 2, ',', '.').' </td>';
                    //Quantidade já Executada
                    $html_linhas .= '<td> '.number_format($dadosCalculados['sumQtdValid'], 2, ',', '.').' </td>';
                    //************Saldo a executar
                    //(%) Supervisão
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoPerc'], 2, ',', '.').' </td>';
                    //Valor Executado
                    $html_linhas .= '<td> '.number_format($dadosCalculados['saldoVl'], 2, ',', '.').' </td>';
                    //************Supervisão Atual
                    //Quantidade já Executada
                    $html_linhas .= '<td> 
                                        '.number_format($value['iemiqtdmedido'], 2, ',', '.').'
                                     </td>';
                    //Valor Executado 
                    $html_linhas .= '<td> '.number_format($value['iemivlmedido'], 2, ',', '.').' </td>';
                    //(%) do Item já executado sobre a Obra após Supervisão 
                    $html_linhas .= '<td colspan="2"> '.number_format($value['itempercsobreobra'], 2, ',', '.').' </td>';
                    //************Campos da Validação
                    //Quantidade já Executada
                    $qtd_val = (!empty($value['iemiqtdvalidado']) && $value['iemiqtdvalidado'] != 0.00)? $value['iemiqtdvalidado'] : $value['iemiqtdmedido'];
                    $html_linhas .= '<td> 
                                        '.campo_texto('iemiqtdvalidado_f[]', 'N', $somenteLeitura, '', 7, 18, '###.###.###.###,##', '', 'right', '', 0, '', 
                                        "calculosEvolucao.keyupAction(this, 'FV', 'qtd');",
                                        number_format($qtd_val, 2, ',', '.')).' 
                                     </td>';
                    
                    $vl_val = (!empty($value['iemivlvalidado']) && $value['iemivlvalidado'] != 0.00) ? $value['iemivlvalidado'] : $value['iemivlmedido'];
                    //Valor Executado 
                    $html_linhas .= '<td> '.
                                        '<span>'.number_format($vl_val, 2, ',', '.').'</span>'.
                                        '<input type="hidden" name="iemivlvalidado_f[]" value="'.number_format($value['iemivlmedido'], 2, ',', '.').'">'.
                                    '</td>';
                    //(%) do Item já executado sobre a Obra após Supervisão 
                    $percItemExecObraPosEvo = $value['itempercsobreobra'];
                    $html_linhas .= '<td colspan="2"> '.
//                                        '<span>WWW</span>'.
                                        '<span>'.number_format($percItemExecObraPosEvo, 2, ',', '.').'</span>'.
                                        '<input type="hidden" name="iemipecentvalidado_f[]" value="'.number_format($value['iemipecentmedido'], 2, ',', '.').'">'.
                                    '</td>';
                    $html_linhas .= '</tr>';
                    
                    $linha_totais = ' 
                                    <tr id="linha_totais_f_v">
                                      <th> TOTAL: </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>                                      
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th colspan="2"> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th> &nbsp; </th>
                                      <th colspan="2"> &nbsp; </th>
                                    </tr>
                                ';
                    
                    break;
                default:
                    $html_linhas = '';
                    break;
            }
        }
        else{
            $html_linhas = '';
        }
    }
    
    if(trim($html_linhas) === ''){
        $html_linhas = ' <tr>
                            <td colspan="20" style="color:red; text-align: center"> Nenhum '.$tipo_item.' cadastrado. </td>
                         </tr> ';
    }else{        
        $html_linhas .= $linha_totais;
    }
    return $html_linhas;    
}
/**
 * 
 * @param string $linhas_tipo_d
 * @param string $linhas_tipo_f
 * @param string $formato_detalhamento
 */
function imprimeTabelaDetalhamento($linhas_tipo_d = '', $linhas_tipo_f = '', $formato_detalhamento = 'cad/ed'){
    
    switch ($formato_detalhamento) {
        case 'cad/ed':            
            echo ' 
                <tr>
                    <td colspan="3">

                        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%; height: 100%;">

                            <thead id="head_itens_cronograma">
                                <tr>
                                    <th rowspan="2" colspan="3" >Descrição</th>
                                    <th rowspan="2">Valor (R$)</th>
                                    <th rowspan="2">% Sobre a Obra</th>
                                    <th rowspan="2">Quantidade</th>
                                    <th rowspan="2">Unidade de Medida</th>
                                    <th rowspan="2">Data de Início</th>
                                    <th rowspan="2">Data de Término</th>
                                    <th colspan="2">Execução Acumulada</th>
                                    <th colspan="2">Saldo a executar</th>
                                    <th colspan="4">Supervisão Atual</th>
                                </tr>
                                <tr>
                                    <!--Execução Acumulada-->
                                    <th>(%) do Item já executado</th>
                                    <th>(%) do Item já executado sobre a Obra</th>
                                    <!--Saldo a executar-->
                                    <th>(%) Supervisão</th>
                                    <th>Valor (R$)</th>
                                    <!--Supervisão Atual-->
                                    <th>(%) do Item já executado sobre a Obra após Supervisão</th>
                                    <th>Quantidade Executada</th>
                                    <th>(%) executado</th>
                                    <th>Valor Executado (R$)</th>
                                </tr>
                            </thead>                            
                            <tbody id="body_itens_cronograma">
                                '.$linhas_tipo_d.' 
                            </tbody>
                            
                            <thead id="head_itens_cronograma_f">
                                <tr>
                                   <th colspan="17" style="text-align: center"> Serviços Externos </th>
                                </tr>                           
                                <tr>
                                    <th rowspan="2">Descrição</th>
                                    <th rowspan="2">Valor Total (R$)</th>
                                    <th rowspan="2">Valor Unitário(R$)</th>
                                    <th rowspan="2">% sobre o total de serviços</th>
                                    <th rowspan="2">Quantidade</th>
                                    <th rowspan="2">Unidade de Medida</th>
                                    <th rowspan="2">Data de Início</th>
                                    <th rowspan="2">Data de Término</th>
                                    <th colspan="3">Execução Acumulada</th>
                                    <th colspan="2">Saldo a executar</th>
                                    <th colspan="6">Supervisão Atual</th>
                                </tr>
                                <tr>
                                    <!--Execução Acumulada-->
                                    <th>(%) do Item já executado</th>
                                    <th>(%) do Item já executado sobre o total de serviços</th>
                                    <th>Quantidade já Executada</th>
                                    <!--Saldo a executar-->
                                    <th>(%) Supervisão</th>
                                    <th>Valor (R$)</th>
                                    <!--Supervisão Atual-->
                                    <th>(%) Supervisão</th>
                                    <th>Quantidade já Executada</th>
                                    <th>Valor Executado (R$)</th>
                                    <th colspan="2">(%) do Item já executado sobre a Obra após Supervisão</th>
                                </tr>
                            </thead>
                            <tbody id="itens_servicos_externos">
                                '.$linhas_tipo_f.'
                            </tbody>
                            <tfoot>
                                <tr>
                                    <td colspan="16">&nbsp;</td>
                                </tr>
                            </tfoot>

                        </table>

                    </td>
                </tr>';


            break;
        case 'valida/ed':            
            echo ' 
                <tr>
                    <td colspan="3">
                        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%; height: 100%;">
                            <thead id="head_itens_cronograma_d">
                                <tr>
                                    <th rowspan="2" colspan="3">Descrição</th>
                                    <th rowspan="2">Valor R$</th>
                                    <th rowspan="2">% Sobre a Obra</th>
                                    <th rowspan="2">Quantidade</th>
                                    <th rowspan="2">Unidade de Medida</th>
                                    <th rowspan="2">Data de Início</th>
                                    <th rowspan="2">Data de Término</th>
                                    <th colspan="2">Execução Acumulada</th>
                                    <th colspan="2">Saldo a Executar</th>
                                    <th colspan="4">Supervisão Atual</th>
                                    <th colspan="4">Validação da Supervisão</th>
                                </tr>
                                <tr>       
                                    <!-- Execução Acumulada -->
                                    <th>(%) do Item já executado</th>
                                    <th>(%) do Item já executado sobre a Obra</th>
                                    <!-- Saldo a Executar -->
                                    <th>(%) Supervisão</th>
                                    <th>Valor (R$)</th>
                                    <!-- Supervisão Atual -->
                                    <th>(%) do Item já executado sobre a Obra após Supervisão</th>
                                    <th>Quantidade Executada</th>
                                    <th>(%) executado</th>
                                    <th>Valor Executado</th>
                                    <!-- Validação da Supervisão -->
                                    <th>(%) do Item já executado sobre a Obra após Supervisão</th>
                                    <th>Quantidade Executada</th>
                                    <th>(%) executado</th>
                                    <th>Valor Executado</th>
                                </tr>
                            </thead>                            
                            <tbody id="body_itens_cronograma_d">
                                '.$linhas_tipo_d.' 
                            </tbody>
                            
                            <thead id="head_itens_cronograma_f">
                                <tr>
                                   <th colspan="21" style="text-align: center"> Serviços Externos </th>
                                </tr>                           
                                <tr>
                                    <th rowspan="2">Descrição</th>
                                    <th rowspan="2">Valor Total (R$)</th>
                                    <th rowspan="2">Valor Unitário (R$)</th>
                                    <th rowspan="2">% Sobre o total de serviços</th>
                                    <th rowspan="2">Quantidade</th>
                                    <th rowspan="2">Unidade de Medida</th>
                                    <th rowspan="2">Data de Início</th>
                                    <th rowspan="2">Data de Término</th>
                                    <th colspan="3">Execução Acumulada</th>
                                    <th colspan="2">Saldo a executar</th>
                                    <th colspan="4">Supervisão Atual</th>
                                    <th colspan="4">Validação da Supervisão</th>
                                </tr>
                                <tr>
                                    <!-- Execução Acumulada -->
                                    <th>(%) do Item já executado</th>
                                    <th>(%) do Item já executado sobre a Obra</th>
                                    <th>Quantidade já Executada</th>
                                    <!-- Saldo a Executar -->
                                    <th>(%) Supervisão</th>
                                    <th>Valor (R$)</th>
                                    <!-- Supervisão Atual -->
                                    <th>Quantidade já Executada</th>
                                    <th>Valor Executado</th>
                                    <th colspan="2">(%) do Item já executado sobre a Obra após Supervisão</th>
                                    <!-- Validação da Supervisão -->
                                    <th>Quantidade já Executada</th>
                                    <th>Valor Executado</th>
                                    <th colspan="2">(%) do Item já executado sobre a Obra após Supervisão</th>
                                </tr>
                            </thead>
                            <tbody id="itens_servicos_externos">
                                '.$linhas_tipo_f.'
                            </tbody>
                            <tfoot>
                                <tr>
                                    <td colspan="15">&nbsp;</td>
                                </tr>
                            </tfoot>
                        </table>
                    </td>
                </tr>';
            break;
        default:
            break;
    }
}

?>