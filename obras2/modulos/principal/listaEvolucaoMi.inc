<?php
$obrid = !empty($_SESSION['obras2']['obrid']) ? $_SESSION['obras2']['obrid'] : $_GET['obrid'];
if(empty($obrid)){
    verificaSessao('obra');    
}

$obr = new Obras($obrid);

$_SESSION['obras2']['empid'] = $obr->empid;
$_SESSION['obras2']['obrid'] = $obr->obrid;

// empreendimento || obra || orgao
verificaSessao('obra');

//Tela de Impressão dos dados da Evolução MI
if($_GET['acao'] == 'I'){
    $emiid = $_GET['emiid'];
    listaImpressaoEvolucaoMI($obrid,$emiid);
}
$desabilita = "";
if(possui_perfil(PFLCOD_CALL_CENTER)){
   $desabilita = "disabled= 'disable'";
}

include APPRAIZ . "includes/cabecalho.inc";

echo '<script language="JavaScript" src="../includes/funcoes.js"></script>
      <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
      <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

$db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros);
echo cabecalhoObra($obrid);
echo alertaObraMi($obrid);

monta_titulo('Lista de Evolução MI', '');

if(isset($_SESSION['obras2']['evoMiStatus'])){unset($_SESSION['obras2']['evoMiStatus']);}

?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
    
var obrid = <?php echo $obrid;?>;

$(document).ready(function(){

    $('.novaEvolucao').click(function(){
        window.location.href = 'obras2.php?modulo=principal/cadEvolucaoMi&acao=A';
    });
    
    $('.goObra').click(function(){
        window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A&obrid='+obrid;
    });

    editarEvolucaoMi = function(emiid){
         window.location.href = 'obras2.php?modulo=principal/cadEvolucaoMi&acao=E&emiid='+emiid;
    }
    
    validarEvolucaoMi = function(emiid){
         window.location.href = 'obras2.php?modulo=principal/cadEvolucaoMi&acao=V&emiid='+emiid;
    }
    
    imprimirEvolucaoMi = function(emiid){
        var url = 'obras2.php?modulo=principal/listaEvolucaoMi&acao=I&emiid='+emiid;
        var popup = window.open(
                        url,
                        "Dados Evolução MI",
                        "width=1300,height=750,scrollbars=yes,scrolling=no,resizebled=no"
                    );
    }
    
    excluirEvolucaoMi = function(emiid){
        var c = confirm('Excluir a Evolução Mi '+emiid+' ?');
        if(c){
            alert('Excluindo...');
            window.location.href = 'obras2.php?modulo=principal/cadEvolucaoMi&acao=A&req=excluir&emiid='+emiid;
        }
    }
});

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="listaEvolucaoMi">
    <thead>
        <tr>
            <th style="width: 70px">Ação</th>
            <th>Data da Evolução</th>
            <th>Data de Inclusão</th>
            <th>Responsável</th>
            <!--  (Edificação) -->
            <th>% Medido (Edificação)</th>
            <th>% Validado (Edificação)</th>
            <th>Valor Medido (R$) (Edificação)</th>
            <th>Valor Validado (R$) (Edificação)</th>
            <!--  (Serviços Externos) -->
            <th>% Medido (Serviços Externos)</th>
            <th>% Validado (Serviços Externos)</th>
            <th>Valor Medido (R$) (Serviços Externos)</th>            
            <th>Valor Validado (R$) (Serviços Externos)</th>            
            <th>Possui Foto</th>
            <th>Situação</th>
            <th>Última tramitação</th>
        </tr>
    </thead>
    <tbody>
    <?php
    
        $param['where']['obrid'] = $obrid;
        $param['where']['emistatus'] = 'A';
        $evmi = new EvolucaoMi();
        $lista = $evmi->listaArray($param);
        
        if (empty($lista)) {
            echo '<tr>
                      <td colspan="15" style="color:red; text-align: center"> Nenhuma evolução cadastrada. </td>
                  </tr>';
        } else {
            $counter = 0;
            $str_bg  = '';
            foreach ($lista as $k => $v) {
                $counter++;
                $str_bg = ($counter % 2 == '') ? '' :' bgcolor="#FFFFFF" ';
                
                $datae = explode('-', $v['emidtevolucao']);
                $datae = $datae[2].'/'.$datae[1].'/'.$datae[0];
                
                $datai = explode('-', $v['emidtinclusao']);
                $datai = $datai[2].'/'.$datai[1].'/'.$datai[0];
                
                echo ' 
                    <tr'.$str_bg.'>
                        <td style="width: 50px">'.$v['acao'].'</td>
                        <td style="text-align: center">'.$datae.'</td>
                        <td style="text-align: center">'.$datai.'</td>
                        <td style="text-align: center">'.$v['usunome'].'</td>
                        <!--  (Edificação) -->
                        <td style="text-align: center">'.number_format($v['emipecentmedidoedif'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emipecentvalidadoedif'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emivlmedidoedif'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emivlvalidadoedif'], 2, ',','.').'</td>
                        <!--  (Serviços Externos) -->
                        <td style="text-align: center">'.number_format($v['emipecentmedidoext'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emipecentvalidadoext'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emivlmedidoext'], 2, ',','.').'</td>                            
                        <td style="text-align: center">'.number_format($v['emivlvalidadoext'], 2, ',','.').'</td>                            
                        <td style="text-align: center">'.$v['possuifoto'].'</td>
                        <td style="text-align: center">'.$v['situacao'].'</td>
                        <td style="text-align: center">'.$v['ultima_tramitacao'].'</td>
                    </tr>
                  ';
            }
        }
    ?>
    </tbody>
    <tfoot>
        <tr>
            <th colspan="15">&nbsp;</th>
        </tr>
        <tr>
            <td colspan="15"> &nbsp; </td>
        </tr>
        <tr>
            <td colspan="14" style="text-align: center"> 
                <input type="button" <?php echo $desabilita; ?> value="Cadastrar Nova Evolução" class="novaEvolucao">                 
                <input type="button" value="Voltar para a Obra"     class="goObra">                 
            </td>
        </tr>
    </tfoot>
</table>


<?php
function listaImpressaoEvolucaoMI($obrid,$emiid){
    $_SESSION['obras2']['evoMiStatus'] = 'impressao';
    $param['emiid'] = $emiid;
    $evmi = new EvolucaoMi();
?>
        <html>
            <head>
                <title> <?php echo NOME_SISTEMA; ?> </title>
            </head>
            <script type="text/javascript" src="/includes/funcoes.js"></script>
            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
            <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
            <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
            <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
            <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
            
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
                    
                    /*
                    this.calculaLinhaTotaisDV = function (){

                        var linha;
                        var sum_coluna01 = 0;
                        var sum_coluna02 = 0;
                        var sum_coluna08 = 0;
                        var sum_coluna10 = 0;
                        var sum_coluna11 = 0;
                        var sum_coluna13 = 0;
                        var sum_coluna14 = 0;
                        var sum_coluna15 = 0;
                        var sum_coluna18 = 0;
                        var objCalc      = this;

                        $('#'+this.objTbodyCalc.attr('id')+' > tr').each(function( index ) {
                        linha = $(this);
                            if(linha.attr('id') !== 'linha_totais_d'){
                                sum_coluna01 +=  (linha.find('td:eq(1)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(1)').text()) : 0;
                                sum_coluna02 += (linha.find('td:eq(2)').text().trim() !== '')  ? objCalc.strVlToFloat(linha.find('td:eq(2)').text()) : 0;
                                sum_coluna08 += (linha.find('td:eq(8)').text().trim() !== '')  ? objCalc.strVlToFloat(linha.find('td:eq(8)').text()) : 0;
                                sum_coluna10 += (linha.find('td:eq(10)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(10)').text()) : 0;
                                sum_coluna11 += (linha.find('td:eq(11)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(11)').text()) : 0;
                                sum_coluna14 += (linha.find('td:eq(14)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(14)').text()) : 0;
                                sum_coluna15 += (linha.find('td:eq(15)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(15)').text()) : 0;
                                sum_coluna18 += (linha.find('td:eq(18)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(18)').text()) : 0;
                            }
                        });

                        this.objLinhaTotais.find('th:eq(1)').html(this.floatToStr(sum_coluna01));
                        this.objLinhaTotais.find('th:eq(2)').html(this.floatToStr(sum_coluna02));
                        this.objLinhaTotais.find('th:eq(8)').html(this.floatToStr(sum_coluna08));
                        this.objLinhaTotais.find('th:eq(10)').html(this.floatToStr(sum_coluna10));
                        this.objLinhaTotais.find('th:eq(11)').html(this.floatToStr(sum_coluna11));
                        this.objLinhaTotais.find('th:eq(14)').html(this.floatToStr(sum_coluna14));
                        this.objLinhaTotais.find('th:eq(15)').html(this.floatToStr(sum_coluna15));
                        this.objLinhaTotais.find('th:eq(18)').html(this.floatToStr(sum_coluna18));

                    };

                    this.calculaLinhaTotaisFV = function() {

                        var linha;
                        var sum_coluna01 = 0;
                        var sum_coluna03 = 0;
                        var sum_coluna09 = 0;
                        var sum_coluna12 = 0;
                        var sum_coluna14 = 0;
                        var sum_coluna15 = 0;
                        var sum_coluna17 = 0;
                        var sum_coluna18 = 0;
                        var objCalc = this;

                        $('#' + this.objTbodyCalc.attr('id') + ' > tr').each(function(index) {
                            linha = $(this);
                            if (linha.attr('id') !== 'linha_totais_f_v') {
                                sum_coluna01 += (linha.find('td:eq(1)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(1)').text()) : 0;
                                sum_coluna03 += (linha.find('td:eq(3)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(3)').text()) : 0;
                                sum_coluna09 += (linha.find('td:eq(9)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(9)').text()) : 0;
                                sum_coluna12 += (linha.find('td:eq(12)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(12)').text()) : 0;
                                sum_coluna14 += (linha.find('td:eq(14)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(14)').text()) : 0;
                                sum_coluna15 += (linha.find('td:eq(15)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(15)').text()) : 0;
                                sum_coluna17 += (linha.find('td:eq(17)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(17)').text()) : 0;
                                sum_coluna18 += (linha.find('td:eq(18)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(18)').text()) : 0;
                            }
                        });

                        this.objLinhaTotais.find('th:eq(1)').html(this.floatToStr(sum_coluna01));
                        this.objLinhaTotais.find('th:eq(3)').html(this.floatToStr(sum_coluna03));
                        this.objLinhaTotais.find('th:eq(9)').html(this.floatToStr(sum_coluna09));
                        this.objLinhaTotais.find('th:eq(12)').html(this.floatToStr(sum_coluna12));
                        this.objLinhaTotais.find('th:eq(14)').html(this.floatToStr(sum_coluna14));
                        this.objLinhaTotais.find('th:eq(15)').html(this.floatToStr(sum_coluna15));
                        this.objLinhaTotais.find('th:eq(17)').html(this.floatToStr(sum_coluna17));
                        this.objLinhaTotais.find('th:eq(18)').html(this.floatToStr(sum_coluna18));

                     };
                    */
                   
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
                                sum_coluna18 += (linha.find('td:eq(18)').text().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(18)').text()) : 0;
//                                if(typeof(linha.find('td:eq(18)').find('input').val()) !== 'undefined'){
//                                    sum_coluna18 += (linha.find('td:eq(18)').find('input').val().trim() !== '') ? objCalc.strVlToFloat(linha.find('td:eq(18)').find('input').val()) : 0;
//                                }
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
                     
                    this.atualizaLinhasTotais = function() {
                        if ($('#linha_totais_d_v').length > 0) {
                            this.objLinhaTotais = $('#linha_totais_d_v');
                            this.objTbodyCalc = this.objLinhaTotais.parents('tbody');
                            this.calculaLinhaTotaisDV();
                        }
                        if ($('#linha_totais_f_v').length > 0) {
                            this.objLinhaTotais = $('#linha_totais_f_v');
                            this.objTbodyCalc = this.objLinhaTotais.parents('tbody');
                            this.calculaLinhaTotaisFV();
                        }
                    };

                }

                var calculosEvolucao = new calculosEvolucao();

                $(document).ready(function() {
                    calculosEvolucao.atualizaLinhasTotais();
                });

            </script>

            <body marginheight="0" marginwidth="0">
            <br/>
<?php
    echo monta_cabecalho_relatorio( '95' );
    echo cabecalhoObra($obrid);
    
    echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
            <tr>
                <td colspan="2" bgcolor="#DCDCDC">
                    <center><label class="TituloTela" style="color:#000000;">Dados da Evolução MI</label></center>
                </td>
            </tr>';
    
    $lista = $evmi->listaArray($param);
    $lista = $lista[0];
    
    $datae = explode('-', $lista['emidtevolucao']);
    $datae = $datae[2].'/'.$datae[1].'/'.$datae[0];

    $datai = explode('-', $lista['emidtinclusao']);
    $datai = $datai[2].'/'.$datai[1].'/'.$datai[0];
    
    echo '<tr>';
    echo '<td colspan="2">';    
    echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="dadosEvolucaoMi">
                <tr>
                    <td class="subtitulodireita" width="190px">Data da Evolução</td>
                    <td>'.$datae.'</td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="190px">Data de Inclusão</td>
                    <td>'.$datai.'</td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="190px">Responsável</td>
                    <td>'.$lista['usunome'].'</td> <!-- Pegar o responsável na tabela entidade -->
                </tr>
                <tr>
                    <td class="subtitulodireita" width="190px">% Medido (Edificação)</td>
                    <td>'.number_format($lista['emipecentmedidoedif'], 2, ',','.').'</td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="190px">% Medido (Serviços Externos)</td>
                    <td>'.number_format($lista['emipecentmedidoext'], 2, ',','.').'</td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="190px">% Validado (Edificação)</td>
                    <td>'.number_format($lista['emipecentvalidadoedif'], 2, ',','.').'</td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="190px">% Validado (Serviços Externos)</td>
                    <td>'.number_format($lista['emipecentvalidadoext'], 2, ',','.').'</td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="190px">Valor (R$) (Edificação)</td>
                    <td>'.number_format($lista['emivlmedidoedif'], 2, ',','.').'</td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="190px">Valor (R$) (Serviços Externos)</td>
                    <td>'.number_format($lista['emivlmedidoext'], 2, ',','.').'</td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="190px">Situação</td>
                    <td>'.$lista['situacao'].'</td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="190px">Última tramitação</td>
                    <td>'.$lista['ultima_tramitacao'].'</td>
                </tr>
                </tr>';
    echo '</table>';    
    echo '</td>';
    echo '</tr>';
    echo '  <tr>
                <td colspan="2" bgcolor="#DCDCDC">
                    <center><label class="TituloTela" style="color:#000000;">Detalhamento de Supervisão e Acompanhamento</label></center>
                </td>
            </tr>';
    
    $arItens            = $evmi->getItensEvolucaoMiByEmiidObrid($emiid, $obrid);
    $arItensServExterno = $evmi->getItensEvolucaoMiByEmiidObrid($emiid, $obrid , 'F');
    
    $html_linhas_d = getHtmlLinhasDetalhamento($arItens,            'D');
    $html_linhas_f = getHtmlLinhasDetalhamento($arItensServExterno, 'F');
    
    echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%; height: 100%;">
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
                                '.$html_linhas_d.' 
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
                                '.$html_linhas_f.'
                            </tbody>
                            <tfoot>
                                <tr>
                                    <td colspan="15">&nbsp;</td>
                                </tr>
                                <tr>
                                  <td colspan="20" style="text-align:center;">
                                      <input type="button" value="Fechar" style="cursor: pointer" onclick="window.close();">
                                  </td>
                                </tr>
                            </tfoot>
                        </table>';
    
    die();
}

function getHtmlLinhasDetalhamento($arr_dados = array(), $tipo_dado = 'D') {
    $html_linhas = '';

    if ($tipo_dado == 'F') {
        $tipo_item = 'serviço externo';
    } else {
        $tipo_item = 'item'; //Item cronograma
    }

    $html_linha_vazia = ' <tr>
                                <td colspan="18" style="text-align: center"> &nbsp; </td>
                             </tr> ';
    
    $counter = 0;
    $str_bg = '';
    foreach ($arr_dados as $key => $value) {
        $counter++;
        $str_bg = ($counter % 2 == '') ? '' : ' bgcolor="#FFFFFF" ';
        $value['iemiid'] = (isset($value['iemiid']) && !empty($value['iemiid'])) ? $value['iemiid'] : 0;
        $value['umdeesc'] = (!empty($value['umdeesc'])) ? $value['umdeesc'] : '-';

        $dadosCalculados = $value['dadosCalculados'];

        //Monta as linhas de Edificação
        if ($tipo_dado == 'D') {
            $html_linhas .= '<tr id="ln_icoid_d_' . $value['icoid'] . '" ' . $str_bg . '>';
            //Descrição
            $html_linhas .= '<td colspan="3"> 
                                <input type="hidden" name="icoid_d[]" value="' . $value['icoid'] . '">
                                <input type="hidden" name="iemiid_d[]" value="' . $value['iemiid'] . '">
                                <img src="/imagens/seta_filho.gif" border="0">
                                ' . $value['itcdesc'] . '
                             </td>';
            //Valor R$
            $html_linhas .= '<td> ' . number_format($value['icovlritem'], 2, ',', '.') . '</td>';
            //% Sobre a Obra
            $html_linhas .= '<td> ' . number_format($value['icopercsobreobra'], 2, ',', '.') . '</td>';
            //Quantidade
            $html_linhas .= '<td> - </td>';
            //Unidade de Medida
            $html_linhas .= '<td> - </td>';
            //Data de Início
            $html_linhas .= '<td> ' . formata_data($value['icodtinicioitem']) . '</td>';
            //Data de Término
            $html_linhas .= '<td> ' . formata_data($value['icodterminoitem']) . '</td>';

            //************Execução Acumulada
            //(%) do Item já executado
            $html_linhas .= '<td> ' . number_format($dadosCalculados['sumPercValid'], 2, ',', '.') . ' </td>';
            //(%) do Item já executado sobre a Obra 
            $html_linhas .= '<td> ' . number_format($dadosCalculados['sobreObraPerc'], 2, ',', '.') . ' </td>';
            //************Saldo a executar
            //(%) Supervisão
            $html_linhas .= '<td> ' . number_format($dadosCalculados['saldoPerc'], 2, ',', '.') . ' </td>';
            //Valor Executado
            $html_linhas .= '<td> ' . number_format($dadosCalculados['saldoVl'], 2, ',', '.') . ' </td>';
            //************Supervisão Atual
            //(%) do Item já executado sobre a Obra após Supervisão
            $per_sobre_obra_pos_exec = ($value['iemipecentmedido'] !=0 ) ? $value['itempercsobreobra'] + $dadosCalculados['sobreObraPerc'] : 0;
            $html_linhas .= '<td> '.number_format($per_sobre_obra_pos_exec, 2, ',', '.').' </td>';
            //Quantidade Executada
            $html_linhas .= '<td> - </td>';
            //(%) executado
            $html_linhas .= '<td> 
                                ' . number_format($value['iemipecentmedido'], 2, ',', '.') . '
                             </td>';
            //Valor Executado
            $html_linhas .= '<td> 
                                ' . number_format($value['iemivlmedido'], 2, ',', '.') . '
                             </td>';
            //************Campos da Validação
            //(%) do Item já executado sobre a Obra após Supervisão - Somado com Execução Acumulada
            $percent_val = (!empty($value['iemipecentvalidado']) && $value['iemipecentvalidado'] != 0.00) ? $value['iemipecentvalidado'] : 0;
            $percItemExecObraPosEvo = ($percent_val != 0) ? $value['itempercsobreobra'] + $dadosCalculados['sobreObraPerc'] : 0;
            $html_linhas .= '<td> '.number_format($percItemExecObraPosEvo, 2, ',', '.').' </td>';
            //Quantidade Executada
            $html_linhas .= '<td> - </td>';
            //(%) executado
            $percent_val = $value['iemipecentvalidado'];
            $html_linhas .= '<td> 
                                ' . number_format($percent_val, 2, ',', '.') . '
                             </td>';
            //Valor Executado
            $vl_val = $value['iemivlvalidado'];
            $html_linhas .= '<td> 
                                ' . number_format($vl_val, 2, ',', '.') . '
                             </td>';
            $html_linhas .= '</tr>';

        }
        //Monta as linhas de Serviços Externos
        elseif ($tipo_dado == 'F') {

            $html_linhas .= '<tr id="ln_icoid_f_' . $value['icoid'] . '" ' . $str_bg . '>';
            //Descrição
            $html_linhas .= '<td> 
                            <input type="hidden" name="icoid_f[]"  value="' . $value['icoid'] . '">
                            <input type="hidden" name="iemiid_f[]" value="' . $value['iemiid'] . '">
                            <img src="/imagens/seta_filho.gif" border="0">
                            ' . $value['itcdesc'] . '
                         </td>';
            //Valor Total R$
            $html_linhas .= '<td> ' . number_format($value['icovlritem_total'], 2, ',', '.') . '</td>';
            //Valor Unitário R$
            $html_linhas .= '<td> ' . number_format($value['icovlritem'], 2, ',', '.') . '</td>';
            //% Sobre a Obra
            $html_linhas .= '<td> ' . number_format($value['icopercsobreobra'], 2, ',', '.') . '</td>';
            //Quantidade
            $html_linhas .= '<td> 
                            <input type="hidden" name="itcquantidade[]" value="' . $value['itcquantidade'] . '">
                            ' . number_format($value['itcquantidade'], 2, ',', '.') . '
                         </td>';
            //Unidade de Medida
            $html_linhas .= '<td> ' . $value['umdeesc'] . '</td>';
            //Data de Início
            $html_linhas .= '<td> ' . formata_data($value['icodtinicioitem']) . '</td>';
            //Data de Término
            $html_linhas .= '<td> ' . formata_data($value['icodterminoitem']) . '</td>';

            //************Execução Acumulada
            //(%) do Item já executado
            $html_linhas .= '<td> ' . number_format($dadosCalculados['sumPercValid'], 2, ',', '.') . ' </td>';
            //(%) do Item já executado sobre a Obra 
            $html_linhas .= '<td> ' . number_format($dadosCalculados['sobreObraPerc'], 2, ',', '.') . ' </td>';
            //Quantidade já Executada
            $html_linhas .= '<td> ' . number_format($dadosCalculados['sumQtdValid'], 2, ',', '.') . ' </td>';
            //************Saldo a executar
            //(%) Supervisão
            $html_linhas .= '<td> ' . number_format($dadosCalculados['saldoPerc'], 2, ',', '.') . ' </td>';
            //Valor Executado
            $html_linhas .= '<td> ' . number_format($dadosCalculados['saldoVl'], 2, ',', '.') . ' </td>';
            //************Supervisão Atual
            //Quantidade já Executada
            $html_linhas .= '<td> 
                            ' . number_format($value['iemiqtdmedido'], 2, ',', '.') . '
                         </td>';
            //Valor Executado 
            $html_linhas .= '<td> ' . number_format($value['iemivlmedido'], 2, ',', '.') . ' </td>';
            //(%) do Item já executado sobre a Obra após Supervisão 
            $percent_val = (!empty($value['iemipecentmedido']) && $value['iemipecentmedido'] != 0.00) ? $value['iemipecentmedido'] : 0;
            $percItemExecObraPosEvo = ($percent_val != 0) ? $value['itempercsobreobra'] + $dadosCalculados['sobreObraPerc'] : 0;
            $html_linhas .= '<td colspan="2"> ' .
                                '<span>' . number_format($percItemExecObraPosEvo, 2, ',', '.') . '</span>' .
                            '</td>';
            
            //************Campos da Validação
            //Quantidade já Executada
//            $qtd_val = (!empty($value['iemiqtdvalidado']) && $value['iemiqtdvalidado'] != 0.00) ? $value['iemiqtdvalidado'] : $value['iemiqtdmedido'];
            $qtd_val = $value['iemiqtdvalidado'];
            $html_linhas .= '<td> 
                                ' . number_format($qtd_val, 2, ',', '.') . ' 
                             </td>';

            $vl_val = $value['iemivlvalidado'];
            //Valor Executado 
            $html_linhas .= '<td> ' .
                                '<span>' . number_format($vl_val, 2, ',', '.') . '</span>' .
                            '</td>';
            //(%) do Item já executado sobre a Obra após Supervisão 
            $percent_val = (!empty($value['iemipecentvalidado']) && $value['iemipecentvalidado'] != 0.00) ? $value['iemipecentvalidado'] : 0;
            $percItemExecObraPosEvo = ($percent_val != 0) ? $value['itempercsobreobra'] + $dadosCalculados['sobreObraPerc'] : 0;
            $html_linhas .= '<td colspan="2"> ' .
                                '<span>' . number_format($percItemExecObraPosEvo, 2, ',', '.') . '</span>' .
                            '</td>';
            $html_linhas .= '</tr>';

            
        } else {
            $html_linhas = '';
        }
        
    }
    
    if ($tipo_dado == 'D') {
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
    }elseif ($tipo_dado == 'F') {
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
    }

    if (trim($html_linhas) === '') {
        $html_linhas = ' <tr>
                            <td colspan="17" style="color:red; text-align: center"> Nenhum ' . $tipo_item . ' cadastrado. </td>
                         </tr> ';
    } else {
        $html_linhas .= $linha_totais;
    }
    return $html_linhas;
}
?>