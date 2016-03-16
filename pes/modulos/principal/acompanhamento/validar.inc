<?php

$entCodigo = $_SESSION['pes']['codigo_entidade'];

switch ($_POST['action']) :
    case 'validar':

        $model = new Model_Validacao();
        if(isset($_POST['value']) && !empty($_POST['value']) && is_array($_POST['value'])){
            $msgValidacao = array();
            foreach ($_POST['value'] as $value) {
                if ($value) {
                    $model->clearEntity();

                    $entitys = $model->getAllByValues(array('entcodigo' => $entCodigo, 'tidcodigo' => $value['tidcodigo'], 'valmes' => $value['valmes'], 'valano' => AEXANO), array('valdata'));
                    if ($entitys) {
                        
                        // Pega o ultimo array de entidades ja retirando do array para deletar os que sobraram logo em seguida.
                        $model->populateEntity(array_pop($entitys));
                        
                        foreach($entitys as $entity)
                            $model->delete($entity['valcodigo']);
                        
                    } else {
                        $model->entity['tidcodigo'] = $value['tidcodigo'];
                        $model->entity['valmes'] = $value['valmes'];
                        $model->entity['valano'] = AEXANO;
                        $model->entity['entcodigo'] = $entCodigo;
                    }
                    
                    if($value['valvalor']){
                        $model->entity['valvalor'] = str_replace(array('.', ',', '-'), array('', '.', '0'), trim($value['valvalor']));
                    } else {
                        $model->entity['valvalor'] = 0;
                    }
                    
                    $model->entity['valstatus'] = $value['valstatus'];
                    $model->entity['valdata'] = 'now()';
                    $model->entity['usucpf'] = "{$_SESSION['usucpforigem']}";

                    // Resultado para validar se tem valor.
                    $valoresDespesa = $model->carregarValorDespesa($model->entity['entcodigo'] , $model->entity['tidcodigo'] , $model->entity['valano'] , $model->entity['valmes']);
                    
                    $isValid = true;
                    if($valoresDespesa){
//                        ver($valoresDespesa);
                        foreach($valoresDespesa as $key => $valorDesepsa){
//                            ver($valorDesepsa,d);
                            if($valorDesepsa['fisicopreenchido'] < 1){
                                $isValid = false;
//                                $msgValidacao[$valorDesepsa['295']][$valorDesepsa['mes']] = utf8_encode("Não foi possível validar o Mês {$valorDesepsa['lconome']} de {$valorDesepsa['tidnome']}, pois não foi informado o dado físico no contrato {$valorDesepsa['contitulo']}");
                                $msgValidacao[] = array('mesnome' => $valorDesepsa['lconome'], 
                                                        'tipocontratonome' => $valorDesepsa['tidnome'],
                                                        'contratonome' => $valorDesepsa['contitulo'],
                                                        'ano' =>$valorDesepsa['ano'],
                                                        'concodigo' => $valorDesepsa['concodigo'],
                                                        'cndcodigo' => $valorDesepsa['cndcodigo'],
                                                        'natcodigo' => $valorDesepsa['natcodigo'],
                                                        'tidcodigo' => $valorDesepsa['tidcodigo']
                                                );
                            }
                        }
                    }
                    
                    if($isValid){
                        try {
                            $result = $model->save();
                        } catch (Exception $exc) {
//                            echo $exc->getTraceAsString();
                        }

//                        ver($model->entity, $result);
//                        echo $result;
                    }
                }
            }
//            pes.php?modulo=principal/acompanhamento/despesas&acao=A&concodigo=295&tidcodigo=6
            if($msgValidacao){
                $html = "Algumas despesas estão pendentes de validação, confira na listagem abaixo: ";
                            $html .= "<table width=\"100%\" align=\"center\" bgcolor=\"#f5f5f5\" >";
                            $html .= "<thead>";
                                $html .= "<tr>";
                                $html .= "<td>Ação</ td>";
                                $html .= "<td>Tipo do contrato</ td>";
                                $html .= "<td>Contrato</ td></ tr>";
                                $html .= "<td>Mês</ td>";
                                $html .= "<td>Ano</ td>";
                                $html .= "</ tr>";
                            $html .= "</thead>";
                            $html .= "<tbody>";
                            foreach($msgValidacao as $msg){
                                $html .= "<tr class=\"list\">";
                                $html .= "<td >";
                                if($msg['tidcodigo'] == K_DESPESA_MATERIAL_CONSUMO){
                                    $html .= '<a style="margin: 0 -5px 0 5px;" 
                                                href="
                                                /pes/pes.php?modulo=principal/acompanhamento/despesas&acao=A&concodigo=' . $msg['cndcodigo'] . '&tidcodigo=' . $msg['tidcodigo'] . '&natcodigo=' . $msg['natcodigo'] . '" 
                                                class="alterar">
                                                <img src="/imagens/alterar.gif" border=0 title="Alterar">
                                              </a>';
                                } else {
                                    $html .= '<a style="margin: 0 -5px 0 5px;" 
                                                href="
                                                /pes/pes.php?modulo=principal/acompanhamento/despesas&acao=A&concodigo=' . $msg['concodigo'] . '&tidcodigo=' . $msg['tidcodigo'] . ' "
                                                class="alterar">
                                                <img src="/imagens/alterar.gif" border=0 title="Alterar">
                                              </a>';
                                }
                                $html .= "</td>";
                                $html .= "<td >";
                                $html .= $msg['tipocontratonome'];
                                $html .= "</td>";
                                $html .= "<td>";
                                $html .= $msg['contratonome'];
                                $html .= "</td>";
                                $html .= "<td>";
                                $html .= $msg['mesnome'];
                                $html .= "</td>";
                                $html .= "<td>";
                                $html .= $msg['ano'];
                                $html .= "</td>";
                                $html .= "</ tr>";
                            }
                            $html .= "</tbody>";
                            $html .= "</ table>";
                            echo $html;
            }
            
            
            
            
//            echo simec_json_encode($msgValidacao);
        }

        exit;
        break;
    case 'confirmar':
        exit;
        break;
    case 'salvarRejeicao':
        $model = new Model_ValidacaoParecer();


        $responsaveis = $model->carregarResponsaveisPorEntidade($entCodigo);
        if($responsaveis){


            //Gera historico dos valores _______________________________________ Gera Historico
            $modelValidacaoHistorico = new Model_ValidacaoHistorico();
            $resultHistorico = $modelValidacaoHistorico->gerarHistorico($entCodigo);

            if($resultHistorico['status']){

                // Salva os valores ________________________________________________ Salva na tabela os dados de envio
                $model->populateEntity($_POST);
                $model->entity['entcodigo'] = $entCodigo;
                $model->entity['vapstatus'] = 'REP';
                $model->entity['vapano'] = AEXANO;
                $model->entity['vapdata'] = 'now()';
                $model->entity['usucpf'] = "{$_SESSION['usucpforigem']}";
                $resultSave = $model->save();


                if($resultSave){



                // Preparando emails para envio ____________________________________ Envia email
                $emailsDest = array();
                foreach($responsaveis as $responsavel){

                    // Verifica e-mail para nao ter email repetidos
                    if(!in_array($responsavel['usuemail'], $emailsDest)){
                        $emailsDest[] = $responsavel['usuemail'];
                    }


//                    // Verifica e-mail para nao ter email repetidos
//                    if($emails){
//                        $validarEmailRepetido = true;
//                        foreach($emails as $email){
//                            if($email == $responsavel['usuemail']){
//                                $validarEmailRepetido = false;
//                            }
//                        }
//
//                        if($validarEmailRepetido) $emails[] = $responsavel['usuemail'];
//
//                    } else {
//                        $emails[] = $responsavel['usuemail'];
//                    }
                }

                // Dados FAKE
                //

                $emailsReme = array('nome' => $_SESSION['usunome'] , 'email' => $_SESSION['usuemail']);

                if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
                    $emailsDest = array();
                    $emailsDest[] = 'ruyjfs@gmail.com';
                    $emailsDest[] = 'orion.mesquita@mec.gov.br';
                }

                $txt = utf8_decode($_POST['vapparecer']);
                $txt = "Prezado(a) Senhor(a) Cadastrador(a) de UO,
                        <br />
                        Informa-se que parte dos dados cadastrados não foram aceitos pelo Líder de UO (Reitor/Pró Reitor). Pede-se, portanto, que as informações sejam retificadas.
                        <br />
                        A fim de identificar quais despesas e/ou meses estão em desacordo, segundo o Líder, favor acesse no módulo \"Esplanada Sustentável\" o menu \"Principal-Validação\", todos os campos que foram recusados estarão assinalados em vermelho, e em seguida realize as alterações solicitadas.
                        <br />
                        Informa-se ainda que o seu Líder relacionou informações complementares, conforme parecer abaixo:
                        <br />
                        <br />
                        <br />
                        Parecer de Líder de UO
                        <br />
                                {$txt}.
                        <br />
                        <br />
                        <br />
                        Atenciosamente,
                        <br />
                        Equipe Esplanada Sustentável - SIMEC";

                // Envia email aos destinatarios
                enviar_email($emailsReme, $emailsDest, 'Acompanhamento de Despesas', $txt , null , array('orion.mesquita@mec.gov.br'));

                    $return = array('status' => true , 'msg' => utf8_encode(MSG011), 'result' => $resultSave);
                } else {
                    $return = array('status' => false , 'msg' => utf8_encode(MSG012 . ' Pois não pode realizar o cadastro dos dados de envio!'), 'result' => $resultSave);
                }
            } else {
                $return = array('status' => false , 'msg' => utf8_encode($resultHistorico['msg']), 'result' => $resultSave);
            }
        } else {
            $return = array('status' => false , 'msg' => utf8_encode(MSG012 . ' Pois não existe nenhum cadastrador nesta entidade!'));
        }

        echo simec_json_encode($return);
        exit;

    break;
    case 'rejeitar':
        ?>

        <form method="post" name="formulario_gravar" id="form_save">
            <input name="action" value="salvarRejeicao" type="hidden" />
            <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <tr>
                    <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Parecer do líder da Unidade Orçamentária:</td>
                    <td>
                        <?php
                        echo campo_textarea('vapparecer', 'S', 'S', '', 70, 13, 2000, null, null, null, null, null, null, array('id' => 'vapparecer'));
                        ?>
                    </td>
                </tr>
            </table>
            <table id="container_botao_enviar" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
                <tr bgcolor="#dcdcdc">
                    <td style="text-align:center">
                        <input onclick="javascript:enviarRejeicao();" type="button" value="Enviar"/>
                    </td>
                </tr>
            </table>
            <script type="text/javascript">
                function enviarRejeicao()
                {
                    var vapparecer = $('#vapparecer');

                    if(vapparecer. val() == ''){
                        msg(vapparecer, 'É necessário preencher o campo <b>Parecer</b>.');
                        return false;
                    }

                    saveSubmitAjax();
                }

                function returnSaveSucess(){
//                    window.location.reload();
                }
            </script>
            <?php
            break;
        default:

        //ver(file_exists('../includes'));
            ?>
        <!--<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
        <script src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>
        <script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
        <link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui/custom-theme/jquery-ui-1.10.3.custom.css" rel="stylesheet">
        <link href="../css/estilo.css" rel="stylesheet" type="text/css" />-->

            <?php
            //Chamada de programa
            include APPRAIZ . "includes/cabecalho.inc";

            // Exibindo barra com os dados recursivos da entidade.
            $controllerUsuario = new Controller_Usuario();
            echo $controllerUsuario->barraEntidadeUsuarioAction();
            echo '<br />';

            monta_titulo($titulo_modulo, '&nbsp;');

            $model = new Model_Validacao();

            if($entCodigo)
                $result = $model->carregarDespesasPorEntidade($entCodigo);
            else $result = null;

            $despesasTr = array();
            $despesasTh = array();

            if ($result) {

                $arrStatusMes = array();
                foreach ($result as $arrValue) {
                    $despesasTr[$arrValue['tidcodigo']][$arrValue['lcocodigo']] = $arrValue;
                    $despesasTh[$arrValue['lcocodigo']]['nome'] = $arrValue['lconome'];
                    $despesasTh[$arrValue['lcocodigo']]['codigo'] = $arrValue['lcocodigo'];
                    $arrStatusMes[$arrValue['lcocodigo']][$arrValue['tidcodigo']] = $arrValue['status'];
                }

                foreach ($arrStatusMes as $codigoMes => $arrStatus) {
                    $classThAnterior = '';
                    foreach ($arrStatus as $status) {

                        if ($classThAnterior) {
                            if ($status != $classThAnterior) {
                                $despesasTh[$codigoMes]['status'] = 'NI';
                            }
                        } else {
                            $despesasTh[$codigoMes]['status'] = $status;
                        }

                        $classThAnterior = $status;
                    }
                }
            }

            function getClassByStatus($status) {
                if ($status == 'REP') {
                    $class = 'red';
                } else if ($status == 'APR') {
                    $class = 'green';
                } else {
                    $class = 'white';
                }

                return $class;
            }

//ver($result);
            ?>
            <style>
                .tabela_filha{
                    border-collapse: collapse;
                    border: 0px solid red; padding: 0px; margin: 0px;
                    width: 100%;
                }
                .td_acao {
                    /*border: 2px solid blue;*/
                    border-right: 2px solid #e0e0e0;
                    border-bottom: 2px solid #e0e0e0;
                    padding: 0px;
                    margin: 0px;
                }

                .td_nome {
                    /*border: 2px solid blue;*/
                    border-right: 0px solid #e0e0e0;
                    border-bottom: 2px solid #e0e0e0;
                    padding: 0px;
                    margin: 0px;
                }

                /*.tabela_filha tbody {border: 1px solid blue; padding: 0px; margin: 0px;}*/
                .tabela_filha tr {
                    border: 0px solid red;
                    padding: 0px;
                    margin: 0px;
                    width: 100%;
                    height: 20px;
                    background-color: #fafafa;
                }

                .linha_listagem{
                    background-color: #fafafa;
                }

                .linha_listagem:hover{
                    background-color: #ffffcc;
                }

                a {
                    color: #000;
                }

                a:hover{
                    color: #000;
                    text-decoration:none;
                }

                thead tr{
                    background-color: #e0e0e0;
                    text-align: center;
                }

                tbody .list:hover {
                    background-color: #e0e0e0 !important;
                }

                td .red:hover{
                    <?php if($controllerUsuario->permission() < 3 ): ?>
                    cursor: pointer;
                    <?php endif ?>
                    background-color: #FF6A6A;
                }

                td .green:hover{
                    <?php if($controllerUsuario->permission() < 3 ): ?>
                    cursor: pointer;
                    <?php endif ?>
                    background-color: #7CCD7C;
                }

                td .red{
                    background-color: #FFC1C1;
                }

                td .green{
                    background-color:#90EE90;
                }

                td .white{
                    background-color:#FFFFFF;
                }

                td .white:hover{
                    <?php if($controllerUsuario->permission() < 3 ): ?>
                    cursor: pointer;
                    <?php endif ?>
                    background-color:#EEE9E9;
                }
            </style>
            <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <?php if ($result): ?>
                    <thead>
                        <tr>
                            <td style=" width: 300px;">Nome da Despesa</td>
                            <?php foreach ($despesasTh as $despesaTh): $class = getClassByStatus($despesaTh['status']); ?>
                                <td style="background-color: #e0e0e0;" class="mes_<?php echo $despesaTh['codigo'] . ' ' . $class ?>" onclick="javascript:checkTr(this);"><?php echo $despesaTh['nome'] ?></td>
                            <?php endforeach ?>
                            <td style="background-color: #e0e0e0;" class="total_despesa">Total</td>
                        </tr>
                    </thead>
                    <tbody>
                        <?php
//                        ver($despesasTr);
                        $totalColuna = array();
                        foreach ($despesasTr as $despesaTr):
                            $nColuna = 0;
                            ?>
                            <tr class="list" >
                                <?php
                                $totalLinha = 0;
                                foreach ($despesaTr as $keyDespesaColuna => $valueDespesaColuna):

                                    $totalLinha += $valueDespesaColuna['total'];
                                    $totalColuna[$keyDespesaColuna] += $valueDespesaColuna['total'];

                                    $class = getClassByStatus($valueDespesaColuna['status']);
                                    ?>
                                    <?php if ($nColuna < 1): $nColuna++ ?>
                                        <td style="text-align: left; background-color: #e0e0e0;"><?php echo $valueDespesaColuna['tidnome'] ?></td>
                                    <?php endif ?>
                                    <td style="text-align: right;" id="<?php echo "{$nColuna}_{$valueDespesaColuna['lcocodigo']}_{$valueDespesaColuna['tidcodigo']}_{$valueDespesaColuna['valcodigo']}" ?>" onclick="javascript:checkTd(this);" class="mes_<?php echo $valueDespesaColuna['lcocodigo'] . ' ' . $class ?> "><?php echo ($valueDespesaColuna['total']) ? number_format($valueDespesaColuna['total'], 2, ',', '.') : '-' ?></td>
                                <?php endforeach ?>
                                <td style="text-align: right; font-weight: bold;"><?php echo ($totalLinha) ? number_format($totalLinha, 2, ',', '.') : '-' ?></td>
                            </tr>
                        <?php endforeach ?>
                        <tr>
                            <td style=" width: 300px; font-weight: bold;">Total</td>
                            <?php foreach ($despesaTr as $keyDespesaColuna => $valueDespesaColuna): ?>
                                <td style="text-align: right; font-weight: bold;"><?php echo ($totalColuna[$keyDespesaColuna]) ? number_format($totalColuna[$keyDespesaColuna], 2, ',', '.') : '-' ?></td>
                            <?php endforeach ?>
                            <td style="text-align: right; background-color: #e0e0e0; font-weight: bold;"><?php echo number_format(array_sum($totalColuna), 2, ',', '.');?></td>
                        </tr>
<!--                        <tr>
                            <td style="background-color: #e0e0e0; width: 300px;"></td>
                            <?php foreach ($despesasTh as $despesaTh): $class = getClassByStatus($despesaTh['status']); ?>
                                <td style="text-align:center; background-color: #e0e0e0;" class="mes_<?php echo $despesaTh['codigo'] . ' ' . $class ?>">
                                    <input type="button" value="Validar" onclick="javascript:confirmar('<?php echo $despesaTh['codigo'] ?>' , '<?php echo $despesaTh['status'] ?>')"/>
                                </td>
                            <?php endforeach ?>
                            <td style="background-color: #e0e0e0;" class="total_despesa"></td>
                        </tr>-->
                    </tbody>
                    
                    <table id="container_botao_enviar" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
                        <tr bgcolor="#dcdcdc">
                            <td style="text-align:center">
                                <?php if($controllerUsuario->permission() < 3 ): ?>
                                <input id="button_reject" type="button" value="Enviar para correção"/>
                                <?php else : ?>
                                &nbsp;
                                <?php endif ?>
                            </td>
                        </tr>
                    </table>
                    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
                        <tr>
                            <td style="border-width: 3px;" style="width: 32%;">
                                <fieldset>
                                    <legend>Legenda:</legend>
                                    <table>
                                        <tr>
                                            <td class="white" style="cursor: default">BRANCO OU INCOLOR: Não validado pelo Líder da UO</td>
                                        </tr>
                                        <tr>
                                            <td class="green" style="cursor: default">VERDE: Validado pelo Líder de UO</td>
                                        </tr>
                                        <tr>
                                            <td class="red" style="cursor: default">VERMELHO: Recusado pelo Líder da UO</td>
                                        </tr>
                                    </table>
                                </fieldset>
                            </td>
                            <td style="width: 78%;" valign="top">
                                <p>
                                    <b>Observação:</b>
                                    <br />
                                    A tabela acima apresenta um painel gerencial para que o Líder da UO (Pró Reitor/Reitor) possa validar/ratificar ou recusar as informações inseridas pela sua equipe de cadastradores.
                                    <br />
                                    Para validar os valores é muito simples, um clique sobre o valor e pronto! Sua despesa naquele mês já passou pelo crivo da autoridade competente (Líder de UO). Tal clique "pintará" a célula de verde.
                                    <br />
                                    É importante também atentar-se para a legenda, posicionada logo abaixo da tabela, com o indicativo das cores, pois a quantidade de cliques altera o status da despesa, entre <b class="green" style="cursor: default">validado (verde)</b>, <b class="red" style="cursor: default">recusado (vermelho)</b> e <b class="white">não validado (incolor ou branco)</b>, nessa ordem.
                                    <br />
                                    Caso queira validar todo o mês, é só clicar sobre aquele mês e pronto, você terá validado todas as despesas daquele mês com um único "clique", lembre-se que a quantidade de cliques altera o status.
                                    <br />
                                    A validação deverá ser feita mesmo para àquelas despesas que não possuem valor (zerado) ou de despesa que não tenha sido pactuada, pois nesse caso o Líder de UO se posiciona de acordo com o lançamento (ou ausência desse).
                                    <br />
                                    Para aqueles casos em que o Líder discorde dos valores informados pela equipe de cadastradores, esse poderá enviar um "parecer" para a equipe solicitando a retificação dos dados, bastando para isso, clicar no botão "Enviar para correção" (posicionado ao centro da parte inferior à tabela).
                                    <br />
                                    Assim que o mês for validado completamente <b class="green" style="cursor: default">(totalmente verde)</b> a SPO/MEC ficará ciente da validação e o papel do Líder de UO terá sido cumprido, naquele mês!
                                </p>
                            </td>
                        </tr>
                    </table>
                <?php else: ?>
                    <tr>
                        <td style="background-color: #ffffcc; text-align: center; color: red;" >Não possui despesas!</td>
                    </tr>
                <?php endif ?>
                    <tr>
                        <td>
                            <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
                                <tr>
                                    <th colspan="2" style="text-align: left;">Notas:</th>
                                </tr>
                                <tr>
                                    <td align="justify" style="border:thin; border-style:solid; border-color:#BEBEBE;width:50%">
                                        <?php echo textoUltimaCargaSIAFI(); ?>
                                    </td>
                                    <td>&nbsp;</td>
                                </tr>
                            </table>
                        </td>
                    </tr>
            </table>
            <?php if($controllerUsuario->permission() < 3 ): ?>
            <script type="text/javascript">
                function confirmar(codigo) {
                    var element = $('.mes_' + codigo);

                    var isValid = true;
                    element.each(function(key, value) {
                         if($(value).attr('id')){
                            var elementClass = $(value).attr('class').split(' ');

                            if(elementClass[1] != 'green'){
                                msg(null , "Falta confirmar todos os valores deste mês!");
                                isValid = false;
                                return false;
                            }
                         }
                    });

                    if(isValid){
                        modalAjax('Acompanhamento Mensal Preenchido', {action: 'confirmar' , mes : codigo});
                    }
                }

                function validar(data, reload)
                    {
//                        modalAjax( 'Listagem de pendências para validação', data);
                        
                        var result;
//                        $.post(window.location.href, data, function(html) {
                            $.ajax({
                                    type: "POST",
                                    url: window.location.href,
                                    data: data,
//                                    dataType: 'json',
                                    async: false,
                                    success: function(html) {
                                        if(html.length > 2){
                                            $( "#dialog" ).remove();
                                            $('body').append('<div id="dialog"></div>');
                                            $( "#dialog" ).html(html).dialog({
                                                title: 'Listagem de pendências para validação',
                                                show: {
                                                    effect: "fade",
                                                    duration: 500
                                                },
                                                hide: {
                                                    effect: "fade",
                                                    duration: 500
                                                },
                                                close: function() {
                                                    if(reload){
                                                        window.location.reload();
                                                    }
                                                  },
                                                height: 320,
                                                width: 800
                                            });
                                            result = false;
                                        } else {
                                            result = true;
                                        }
                                    }
                        });
                        
                        return result;
                    }
//                            function validar(data)
//                            {
//                                $.ajax({
//                                    type: "POST",
//                                    url: window.location.href,
//                                    data: data,
//                                    dataType: 'json',
//                                    async: false,
//                                    success: function(html) {
//                                        console.info(html);
//
//                                        //                            if (html['status'] == true) {
//                                        //                                msg(html['msg']);
//                                        //                                returnSaveSucess(html);
//                                        //                            } else {
//                                        //                                var campo = $("#" + html['name']);
//                                        //                                msg(html['msg'], campo);
//                                        //                                $('html, body').animate({scrollTop: campo.offset.top - 300}, 500);
//                                        //                            }
//                                    }
//                                });
//                            }

                            function checkTr(element) {
                                var element = $(element);
                                var elementClass = element.attr('class').split(' ');
                                var status = '';

                                if (elementClass[1] == 'green') {
                                    $('.' + elementClass[0]).removeClass('green');
                                    $('.' + elementClass[0]).addClass('red');
                                    status = 'REP';

                                } else if (elementClass[1] == 'red') {
                                    $('.' + elementClass[0]).removeClass('red');
                                    $('.' + elementClass[0]).addClass('white');
                                    status = 'NI';
                                } else {
                                    $('.' + elementClass[0]).removeClass('white');
                                    $('.' + elementClass[0]).addClass('green');
                                    status = 'APR';
                                }

                                data = {action: 'validar', value: new Array()};

                                $('.' + elementClass[0]).each(function(key, value) {

                                    var elementChild = $(value);
                                    arrId = elementChild.attr('id');

                                    if (arrId) {
                                        arrId = arrId.split('_');
                                        data.value[key] = {valvalor: elementChild.text(), valmes: arrId[1], tidcodigo: arrId[2], valstatus: status, valcodigo: arrId[3]};
                                    }
                                });

                                if(!validar(data, true)){
//                                    windowReload();
                                }
                            }
                            // Por despesa
                            //function checkTr(element){
                            //
                            //    var element = $(element);
                            //    var elementClass = element.parent('tr').children('td').attr('class');
                            //
                            //    if(elementClass == 'green'){
                            //        element.parent('tr').children('td').attr('class', 'red');
                            //    } else if(elementClass == 'red'){
                            //        element.parent('tr').children('td').attr('class', 'white');
                            //    } else {
                            //        element.parent('tr').children('td').attr('class', 'green');
                            //    }
                            //}

                            function checkTd(element) {
                                var element = $(element);
                                var elementClass = element.attr('class').split(' ');
                                arrId = element.attr('id').split('_');

                                //                    var data = new Array();
                                //                    data = array('action' => 'validar');
                                data = {action: 'validar', value: {0: {valvalor: element.text(), valmes: arrId[1], tidcodigo: arrId[2], valcodigo: arrId[3]}}};

                                //                    NI - Não Iniciado
                                //                    APR - Aprovado
                                //                    REP - Reprovado';
                                if (elementClass[1] == 'green') {
                                    data.value[0].valstatus = 'REP';

                                } else if (elementClass[1] == 'red') {
                                    data.value[0].valstatus = 'NI';
                                } else {
                                    data.value[0].valstatus = 'APR';
                                }
                                
                                if(validar(data, false)){
                                    if (elementClass[1] == 'green') {
                                        element.removeClass('green');
                                        element.addClass('red');

                                    } else if (elementClass[1] == 'red') {
                                        element.removeClass('red');
                                        element.addClass('white');
                                    } else {
                                        element.removeClass('white');
                                        element.addClass('green');
                                    }
                                }

                            }

                            $('#button_reject').click(function() {
                                modalAjax('Parecer do líder da Unidade Orçamentária', {action: 'rejeitar'});
                            });

            </script>
            <?php endif ?>
            <?php
            break;
    endswitch;
    ?>