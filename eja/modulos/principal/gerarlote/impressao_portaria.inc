<?PHP

    require (APPRAIZ . 'includes/library/simec/Listagem.php');
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    include_once APPRAIZ . "includes/funcoesspo_componentes.php";

    include APPRAIZ . "includes/cabecalho.inc";

    $lotnumero = $_REQUEST['lotnumero'];


    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }

    if( $lotnumero > 0 ){
        $dados = buscaNumPortaria( $lotnumero );
    }
    
    $sql = "SELECT lesuf, lesrazaosocial,  lescnpj, 
       lesqtaluno, lesvlcusteio,  lesnumparcela, lesvlparcela,lesexercicio
  FROM eja.loteestado
 WHERE lesnumero = '$lotnumero'";

    $loteEstado = $db->carregar($sql);
    

?>

<style type="">
    @media print {
        #print-head, #print-fot{
            display:none;
        }
        .notprint {
            display: none;

        }
        .div_rolagem{
            display: none;
        }
        .div_rol{
            display: none;
        }
    }

    @media screen {
        .notscreen {
            display: none;
        }
        .div_rol{
            display: none;
        }
    }
    .div_rolagem{
        overflow-x: auto;
        overflow-y: auto;
        height: 50px;
    }
    .div_rol{
        overflow-x: auto;
        overflow-y: auto;
        height: 50px;
    }
    .estiloTitulo{
        text-align: center;
    }
    .estiloTexto{
        text-align: justify;
        font-family: sans-serif;
        font-size: 14px;
        line-height:200%;
    }
    .estiloTextoRecuo{
        text-align: justify;
        font-family: sans-serif;
        font-size: 13px;
        line-height:150%;
    }
    .div_info_portaria_data{

    }
    .bnt{
        padding:3px 9px !important;
    }
</style>

<script type="text/javascript">
    var Tleft = 0;
    var Ttopo = 0;

    $( "html" ).mousemove( function( mouse ) {
        Tleft = mouse.pageX - 160;
        Ttopo = mouse.pageY - 250;
    });

    function imprimirPortaria( tipo ){
        if( tipo == 'pre' ){
            $( ".div_info_portaria_data" ).css( "top", Ttopo );
            $( ".div_info_portaria_data" ).css( "left", Tleft );
            $( ".div_info_portaria_data" ).css( "display", "");
        }

        if( tipo == 'imp' ){
            window.print();
        }
    }

    function fecharJanela(){
        $( ".div_info_portaria_data" ).css( "display", "none");
    }

    function salvarDadosPortaria(){
        var erro;
        var campos = '';
        var formulario = document.formulario;

        $.each($(".obrigatorio"), function(i, v){
            if( ($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox') ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else
                if( $(this).attr('type') == 'radio' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var radio_box = $('input:radio[name='+name+']:checked');

                    if(!radio_box.val()){
                        erro = 1;
                        if(value == 'S'){
                            campos += '- ' + $(this).attr('title') + " \n";
                        }
                    }
            }else
                if( $(this).attr('type') == 'checkbox' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var check_box = $('input:checkbox[name='+name+']:checked');

                    if(!check_box.val()){
                        erro = 1;
                        if(value == 'S'){
                            campos += '- ' + $(this).attr('title') + " \n";
                        }
                    }
            }
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarDadosPortaria');
            $('#formulario').submit();
        }
    }

</script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<form name ="formulario" id="formulario" method="POST">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="lotnumero" id="lotnumero" value="<?=$lotnumero?>">

    <div class="row col-lg-12"><!--  -->
        <div class="div_info_portaria_data notprint" style="position:absolute; width:62%; border:1px solid #696969; -webkit-border-radius:10px; -moz-border-radius:15px; border-radius:15px; display:none; background-color: #8FBC8F; z-index:99999;">
            <table bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" width="100%">
                <tr>
                    <td colspan="2"> &nbsp; </td>
                </tr>
                <tr>
                    <td width="30%" style="text-align: right; font-weight: bold;"> Nº da Portaria </td>
                    <td>
                        <?PHP
                            $lotenumportaria = $dados['lotenumportaria'];
                            echo campo_texto('lotenumportaria', 'S', 'S', 'Nº da Portaria', 30, 30, '', '', 'left', '', 0, 'id="lotenumportaria"', '', $lotenumportaria, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td style="text-align: right; font-weight: bold;"> Data da Portaria </td>
                    <td>
                        <?PHP
                            $lotedataportaria = $dados['lotedataportaria'];
                            echo campo_texto('lotedataportaria', 'S', 'S', 'Data da Portaria', 30, 30, '', '', 'left', '', 0, 'id="lotedataportaria"', '', $lotedataportaria, '', null);
                        ?>
                        <br>
                        <span style="text-align: right; font-weight: bold; color: red;">EX: 01 de Julho de 2001</span>
                    </td>
                </tr>
                <tr>
                    <td colspan="2" style="text-align: center;">
                        <button class="btn btn-success" id="bntCriarLote" type="button" onclick="salvarDadosPortaria();">
                            <i class="glyphicon glyphicon-ok"></i> Salvar Nº de Portaria
                        </button>
                         <button class="btn btn-success" id="bntCriarLote" type="button" onclick="fecharJanela();">
                            <i class="glyphicon glyphicon-remove"></i> Fechar
                        </button>
                    </td>
                </tr>
            </table>
        </div>

</form>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" width="98%">
        <tr>
            <td class="estiloTitulo"> <h4><b>MINISTÉRIO DA EDUCAÇÃO</b></h4> </td>
        </tr>
        <tr>
            <td class="estiloTitulo"> <h5>Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão</h5> </td>
        </tr>
        <tr>
            <td class="estiloTitulo"> <h5>Esplanada dos Ministérios, Bloco L, 2º andar - CEP 70047-900 - Brasília, Distrito Federal, Brasil</h5> </td>
        </tr>
        <tr>
            <td class="estiloTitulo"> <h5>Gabinete: Fones (0xx61) 2022-9018 e 2022-9217 - fax (0xx61) 2022-9321</h5> </td>
        </tr>
    </table>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" width="98%">
        <tr>
            <td colspan="2" class="estiloTitulo"> <b>PORTARIA N.º <?=$dados['lotenumportaria'];?>, DE <?=$dados['lotedataportaria']?>.</b> </td>
        </tr>
        <tr>
            <td>&nbsp;</td>
            <td class="estiloTextoRecuo" width="40%"> <br> Divulgar a relação dos entes executores de ações referentes às novas turmas de Educação de Jovens e Adultos - EJA. </td>
        </tr>
    </table>
    <br>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" width="98%">
        <tr>
            <td class="estiloTexto">
                <p>
                    <b>A SECRETARIA DE EDUCAÇÃO CONTINUADA, ALFABETIZAÇÃO, DIVERSIDADE E INCLUSÃO</b>, no uso das atribuições legais conferidas no Decreto nº 7.690 de 2 de março de 2012, e
                    considerando o disposto na Lei nº 10.880, de 09 de junho de 2004, e do Decreto nº6.093, de 24 de abril de 2007, e da Resolução CD/FNDE nº 48, de 2 de outubro de
                    2012, e Resolução/CD/FNDE nº 52, de 11 de outubro de 2013 e da Resolução/CD/FNDE nº48, de 11 de dezembro de 2013, que estabelece orientações, critérios e
                    procedimentos para a transferência automática de recursos financeiros aos estados, municípios e Distrito Federal para a manutenção de novas turmas de Educação de
                    Jovens e Adultos, resolve:
                </p>
                <br>
                <p>
                    Art. 1º - Divulgar a relação dos entes executores - que aderiram à Resolução/CD/FNDE Nº48 11 de 11 de dezembro de 2013 e que cadastraram no Sistema de Monitoramento,
                    Execução e Controle do Ministério da Educação (Simec) a relação nominal de novos alunos da modalidade EJA, validada pela Diretoria de Políticas de Alfabetização e
                    Educação de Jovens e Adultos (DPAEJA) - considerados aptos a receber recursos para a execução de ações referentes as novas turmas de EJA, no exercício de <?php if($loteEstado){echo '2015';}else{ echo '2014';}?>, na
                    forma do Anexo desta Portaria.
                </p>
                <br>
                <p>
                    Art.2º - Esta portaria entra em vigor na data de sua publicação.
                </p>
<br>
                <!--
                <p class="estiloTitulo">
                    _____________________________________________________ <br>
                    MACAÉ MARIA EVARISTO DOS SANTOS <br>
                    Secretário de Educação Continuada, Alfabetização, Diversidade e Inclusão
                </p>
                -->

                <?if((int)$lotnumero > 5){?>
                    <p class="estiloTitulo">
                        _____________________________________________________ <br>
                        PAULO GABRIEL SOLEDADE NACIF <br>
                        Secretário de Educação Continuada, Alfabetização, Diversidade e Inclusão
                    </p>
                <?}else{?>
                    <p class="estiloTitulo">
                        _____________________________________________________ <br>
                        ADRIANO ALMEIDA DANI <br>
                        Secretário de Educação Continuada, Alfabetização, Diversidade e Inclusão - Substituto
                    </p>
                <?}?>

            </td>
        </tr>
    </table>
</div>

<br><br><br>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" width="98%">
    <tr>
        <td class="estiloTitulo"> <h4><b>MINISTÉRIO DA EDUCAÇÃO</b></h4> </td>
    </tr>
    <tr>
        <td class="estiloTitulo"> <h5>Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão</h5> </td>
    </tr>
    <tr>
        <td class="estiloTitulo"> <h5>Esplanada dos Ministérios, Bloco L, 2º andar - CEP 70047-900 - Brasília, Distrito Federal, Brasil</h5> </td>
    </tr>
    <tr>
        <td class="estiloTitulo"> <h5>Gabinete: Fones (0xx61) 2022-9018 e 2022-9217 - fax (0xx61) 2022-9321</h5> </td>
    </tr>
    <tr>
        <td class="estiloTitulo"> <h5>ANEXO</h5> </td>
    </tr>
    <tr>
        <td colspan="2" class="estiloTitulo"> <b>PORTARIA N.º <?=$dados['lotenumportaria'];?>, DE <?=$dados['lotedataportaria']?>.</b> </td>
    </tr>
</table>

<br><br>

<div class="row col-lg-12">

    <?PHP
    
    if(!$loteEstado){
        $sql = "
            SELECT  lotuf,
                    mundescricao,
                    lotmuncod,
                    lotcnpj,
                    lotqtaluno,
                    --trim(to_char(lotvlcusteio, '999G999G999G990D99')) AS lotvlcusteio,
                    lotvlcusteio,
                    lotnumparcela,
                    lotvlparcela,
                    lotexercicio
            FROM eja.lote lot
            INNER JOIN territorios.municipio mun ON mun.muncod = lot.lotmuncod 
            WHERE lotnumero = {$lotnumero}
        ";
        $cabecalho = array("UF", "Município", "Código do IBGE", "CNPJ", "Total Alunos", "Valor Total", "Parcela", "Valor da Parcela", "Exercicio" );
    $colunaAluno  = array("lotqtaluno");
        $colunasTotal = array("lotvlcusteio", "lotvlparcela");
        
        }else{
       $cabecalho = array("UF", "Razão Social", "CNPJ", "Total Alunos", "Valor Total", "Parcela", "Valor da Parcela", "Exercicio" );
        $colunaAluno  = array("lesqtaluno");
        $colunasTotal = array("lesvlcusteio", "lesvlparcela");

    }
        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem ->addCallbackDeCampo($colunasTotal, "mascaraMoeda");
        $listagem ->addCallbackDeCampo($colunaAluno, "mascaraNumero");
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $colunasTotal);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $colunaAluno);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    ?>

</div>

<div class="text-center notprint">
    <button class="btn btn-success" id="bntCriarLote" type="button" onclick="imprimirPortaria('pre');">
        <i class="glyphicon glyphicon-pencil"></i> Informar Dados da Portaria
    </button>

    <button class="btn btn-success" id="bntCriarLote" type="button" onclick="imprimirPortaria('imp');">
        <i class="glyphicon glyphicon-print"></i> Imprimir Portaria
    </button>
</div>