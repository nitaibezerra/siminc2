<?PHP
    header("Cache-Control: no-store, no-cache, must-revalidate");
    header("Cache-Control: post-check=0, pre-check=0", false);
    header("Pragma: no-cache");
    
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }
    
    $fdpcpf = $_SESSION['cpfavaliado'];
    
    #VERIFICA SE EXISTE A SESSÃO. "USUARIO SELECIONADO", CASO NÃO VIRIFICA QUAL O PERFIL E REDIRECIONA PARA A RESPECTIVA PAGINA.
    $perfil = pegaPerfilGeral();

    $permissao = buscaPermissaoReconcideracao( $fdpcpf );
    
    if( $_SESSION['cpfavaliado'] != '' ){
        if( $permissao['permissao'] == 'N' ){
            if( !in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) || !in_array( PERFIL_SUPER_USER, $perfil ) ){
                $db->sucesso( 'principal/avaliacao_servidor/lista_grid_pess_avaliados', '', 'Não é possivel realizar o Pedido de Reconsideração. Selecione o servidor correto!' );
                die();
            }else{
                $db->sucesso( 'principal/avaliacao_servidor/lista_grid_servidores_avaliados', '', 'Não é possivel realizar o Pedido de Reconsideração. Selecione o servidor correto!' );
                die();
            }
        }
    }else{
        if( !(in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ) ){
            $db->sucesso( 'principal/avaliacao_servidor/lista_grid_pess_avaliados', '', 'Não é possivel realizar o Pedido de Reconsideração. Selecione um servidor correto!' );
            die();
        }else{
            $db->sucesso( 'principal/avaliacao_servidor/lista_grid_servidores_avaliados', '', 'Não é possivel realizar o Pedido de Reconsideração. Selecione um servidor correto!' );
            die();
        }
    }

    if( $fdpcpf ){
        $sql = "
            SELECT  ftprid, 
                    fdpcpf, 
                    ftprboletimserv, ftprdtboletimserv, 
                    ftprconmettecdesatcaref, ftprpontrecebida, ftprpontsolicitada,
                    ftprprodtrabalho, ftprpontrecebidaprodtrab, ftprpontsolicprodtrab, 
                    ftprcapautdesen, ftprpontrecebcapdesen, ftprpontsoliccapdesen, 
                    ftprrelacinterpessoal, ftprpontrecebrelintpes, ftprpontsolicrelintpes, 
                    ftprtrabequipe, ftprpontrecebtrabequipe, ftprpontsolictrabequipe, 
                    ftprcomprtrabalho, ftprpontrecebcomptrab, ftprpontsoliccomptrab, 
                    ftprcumpnorproccodatrcarg, ftprpontrecebcumpnorproccodatrcarg, ftprpontsoliccumpnorproccodatrcarg,
                    flprparcerchefia, 
                    ftprpedido,
                    ftprcienavaliador,
                    ftprliberaparecerchefe

            FROM gestaopessoa.ftpedidoreconsideracao 

            WHERE fdpcpf = '$fdpcpf' AND anoreferencia = {$_SESSION['exercicio']}
        ";		
        $arrDados = $db->pegaLinha( $sql );
        $arrDados = $arrDados ? $arrDados : array();
        extract($arrDados);

        $sql = "
            SELECT  ser.sercpf, 
                    ser.sernome, 
                    ser.sersiape, 
                    ser.sercargo,
                    ser.sercodigofuncao,
                    tls.tlssigla, 
                    ser.sercpfchefe,
                    fun.fdftelefone
                    
                    --NAO É POSSIVEL UMA FORMATAÇÃO POS NAO EXISTE UM PADRAO NOS TELEFONES ARMAZENADOS NA BASE.
                    /*
                    CASE WHEN ( substr(fun.fdftelefone, 1, 4) = '2022' AND COUNT(fun.fdftelefone) < 9 ) 
                        THEN TRIM( TO_CHAR( CAST( TRIM ( REPLACE( REPLACE( REPLACE( REPLACE( fun.fdftelefone,' ', ''), '/', '') ,'.','') , '-', '' ) ) as bigint), '9999-9999') ) 
                        ELSE ''
                    END AS fdftelefone 
                    */
            FROM gestaopessoa.servidor AS ser
            LEFT JOIN gestaopessoa.ftdadofuncional AS fun ON fun.fdpcpf = ser.sercpf
            LEFT JOIN gestaopessoa.tipolotacaoservidor AS tls on tls.tlsid = ser.tlsid and tls.tlsstatus = 'A'

            WHERE ser.sercpf = '$fdpcpf' AND ser.seranoreferencia = {$_SESSION['exercicio']}
                
            GROUP BY ser.sercpf, ser.sernome, ser.sersiape, ser.sercargo, ser.sercodigofuncao, tls.tlssigla,  ser.sercpfchefe, fdftelefone
        ";
        $arrServidor = $db->pegaLinha( $sql );

        $sql = "
            SELECT  DISTINCT ser.sercpf, 
                    ser.sernome, 
                    ser.sersiape, 
                    ser.sercargo, 
                    tls.tlssigla, 
                    ser.sercodigofuncao
                    
            FROM gestaopessoa.servidor ser
            LEFT JOIN gestaopessoa.ftdadofuncional AS fun ON fun.fdpcpf = ser.sercpf
            LEFT JOIN gestaopessoa.tipolotacaoservidor AS tls ON tls.tlsid = ser.tlsid AND tls.tlsstatus = 'A'

            WHERE ser.sercpf = '{$arrServidor['sercpfchefe']}' AND ser.seranoreferencia = {$_SESSION['exercicio']}
        ";
        $arrChefe = $db->pegaLinha( $sql );
    }
    
    #VERIFICA SE O USUÁRIO É O CHEFE OU O PROPRIO. CASO SEJA O PROPRIO É HABILITADO OS CAMPOS (DE PEDIDO) PARA EDIÇÃO.
    $tipo_usu = buscaPermissaoReconcideracao( $fdpcpf );
    
    if( $tipo_usu['usuario_tipo'] == 'SU'){
        $habilitado = 'S';
    }else{
        $habilitado = 'N';
    }

    include  APPRAIZ."includes/cabecalho.inc";
    echo '<br>';
    monta_titulo( 'Gestão de Pessoas', 'Formulário de Pedido de Reconsideração' );
    echo '<br>';

    #VARIAVEL E CONTROLE DE ABAS.
    $perfil = pegaPerfilGeral();
        
    if( in_array( PERFIL_SUPER_USER, $perfil ) || in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) ){
        $abacod_tela = ABA_CAD_AVALIACAO_SERVIDOR_ADM;
    }else{
        $abacod_tela = ABA_CAD_AVALIACAO_SERVIDOR;
    }
    
    $url            = 'gestaopessoa.php?modulo=principal/avaliacao_servidor/cad_pedido_reconsideracao&acao=A';
    $parametros     = '';
    
    $db->cria_aba($abacod_tela, $url, $parametros);
    
    #BUSCA O TIPO DE GRATIFICAÇÃO QUE É ATRIBUIDA AO SERVIDOR AVALIADO.
    $tp_gra = verificaTipoGratificacaoServidor( $_SESSION['cpfavaliado'] );
?>

<style type="text/css">
    .textoJustificado{
        font-weight: normal;
        font-size: 11px;
        color: black;
        font-family: Arial, Verdana;
        background-color: #f0f0f0;
        text-align: justify;
        line-height: 19px;
    }    
</style>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

<script type="text/javascript">

    function atualizaStatusLiberaParecer(obj) {
        var status = obj.checked;
        var ftprid = $('#ftprid').val();

        $.ajax({
            type: "POST",
            url : window.location,
            data: "requisicao=atualizaStatusLiberaParecer&status="+status+"&ftprid="+ftprid,
            asynchronous: true,
            success: function( msg ){
                var resp = pegaRetornoAjax('<resp>', '</resp>', msg, true);
                $('#td_libera_analise').html(resp);
                $('#formulario').submit();
            }
        });
 		
    }
    
    function validaValor( obj ){
        var objValor = obj.value;
        var objId = obj.id;
        if( Number(objValor) > 100 ){
            alert('Nota não pode ser maior que 100');
            $('#'+objId).val('0');
            obj.focus();
            return false;
        }
    }
    
    function imprimir(){
        window.open("gestaopessoa.php?modulo=principal/avaliacao_servidor/imp_cad_pedido_reconsideracao&acao=A","Pedido",'scrollbars=yes,height=600,width=1200,status=no,toolbar=no,menubar=no,location=no');
    }

    function gravarPedido(){
        var boVazio = false;
        
        var fdftelefone = $('#fdftelefone');

        $('#formulario input, #formulario textarea').each(function(){
            if( $(this).val() != '' && $(this).attr('readonly') != true && $(this).attr('type') != 'button' ){
                if( $(this).attr('type') == 'checkbox' ){
                    if( $(this).attr('checked') == true ){
                        boVazio = true;
                    }
                } else {
                    boVazio = true;
                }
            }

        });
        
        if( !fdftelefone.val() ){
            alert('O campo "Telefone (Ramal)" é um campo obrigatório!');
            fdftelefone.focus();
            erro = 1;
            return false;
        }
        

        if( !boVazio ){
            alert('Existe campo obrigatório vazio!');
            return false;
        }

        $('#requisicao').val('salvarPedidoReconsideracao');
        $('#formulario').submit();
    }
    
</script>

<form id="formulario" name="formulario" method="post" action="">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="ftprid" id="ftprid" value="<?=$ftprid;?>">
    <input type="hidden" name="fdpcpf" id="fdpcpf" value="<?=$fdpcpf;?>">
    
    <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td colspan="5" class="subTituloCentro" style="font-size: 15px; background-color: #dcdcdc;">
                Formulário de Pedido de Reconsideração - Ciclo Avaliativo: <?=DATA_PERIODO_INICO_AVALIACAO;?> a <?=DATA_PERIODO_FINAL_AVALIACAO;?>
            </td>
        </tr>
        <tr>
            <td colspan="5" class="subTituloCentro" style="font-size: 10px; background-color: #dcdcdc;">
                Gratificação de Desempenho do Plano Geral de Cargos do Poder Executivo - Decreto nº 7.133, de 19 de março de 2010
            </td>
        </tr>

        <tr>
            <td colspan="2" class="subtituloEsquerda" width="45%" style="font-size: 11px; background-color: #e9e9e9;"> Identificação do(a) Servidor(a) </td>
            <td width="1%" style="background-color: #B5B5B5;">&nbsp;</td>
            <td colspan="2" class="subtituloEsquerda" width="54%" style="font-size: 11px; background-color: #e9e9e9;"> Identificação do(a) Avaliador(a) </td>
        </tr>
        
        <tr>
            <!-- DADOS SERVIDOR -->
            <td class="subTituloDireita"> Nome: </td>
            <td> <?=$arrServidor['sernome'];?> </td>
            <td width="1%" style="background-color: #B5B5B5;">&nbsp;</td>
            
            <!-- DADOS CHEFE AVALIADOR -->
            <td class="subTituloDireita"> Nome: </td>
            <td> <?=$arrChefe['sernome'];?> </td>
        </tr>
        <tr>
            <!-- DADOS SERVIDOR -->
            <td class="subTituloDireita"> SIAPE: </td>
            <td> <?=$arrServidor['sersiape'];?> </td>
            <td style="background-color: #B5B5B5;">&nbsp;</td>
            
            <!-- DADOS CHEFE AVALIADOR -->
            <td class="subTituloDireita"> SIAPE: </td>
            <td> <?=$arrChefe['sersiape'];?> </td>
        </tr>
        <tr>
            <!-- DADOS SERVIDOR -->
            <td class="subTituloDireita"> Cargo: </td>
            <td> <?=$arrServidor['sercargo'];?> </td>
            <td style="background-color: #B5B5B5;">&nbsp;</td>
            
            <!-- DADOS CHEFE AVALIADOR -->
            <td class="subTituloDireita"> Cargo: </td>
            <td> <?=$arrChefe['sercargo'];?> </td>
        </tr>
        <tr>
            <!-- DADOS SERVIDOR -->
            <td class="subTituloDireita"> Função: </td>
            <td> <?=$arrServidor['sercodigofuncao'];?> </td>
            <td style="background-color: #B5B5B5;">&nbsp;</td>
            
            <!-- DADOS CHEFE AVALIADOR -->
            <td class="subTituloDireita"> Função: </td>
            <td> <?=$arrChefe['sercodigofuncao'];?> </td>
        </tr>
        <tr>
            <!-- DADOS SERVIDOR -->
            <td class="subTituloDireita"> Unidade de Exercicio: </td>
            <td> <?=$arrServidor['tlssigla'];?> </td>
            <td style="background-color: #B5B5B5;">&nbsp;</td>
            
            <!-- DADOS CHEFE AVALIADOR -->
            <td class="subTituloDireita"> Unidade de Exercicio: </td>
            <td> <?=$arrChefe['tlssigla'];?> </td>
        </tr>
        <tr>
            <!-- DADOS SERVIDOR -->
            <td class="subTituloDireita"> Telefone (Ramal): </td>
            <td> <?=$arrServidor['fdftelefone'];?> </td>
            <td style="background-color: #B5B5B5;">&nbsp;</td>
            
            <!-- DADOS CHEFE AVALIADOR -->
            <td class="subTituloDireita"> Telefone (Ramal): </td>
            <td>
                <?PHP
                    $fdftelefone = $arrChefe['fdftelefone'];
                    echo campo_texto('fdftelefone', 'N', $habilitado, '', 12, 12, '##-####-####', '', '', '', 0, 'id="fdftelefone"', '', $fdftelefone ); 
                ?>
            </td>
        </tr>
    </table>
    
    <?PHP
        $perfil = pegaPerfilGeral();

        if( ( in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ) && $ftprid != '' ){
            
            $ftprliberaparecerchefe = buscaStatusLiberaParecer( $ftprid );

            if( $ftprliberaparecerchefe == 't' ){
                $checked = 'checked="checked"';
                $cor     = 'color:green';
            }else{  
                $checked = "";
                $cor     = "";
            }
    ?>
        <br>
        
        <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td colspan="5" class="subTituloCentro" style="font-size: 12px; background-color: #dcdcdc;">
                    Liberação de Pedido para Análise
                </td>
            </tr>
            <tr>
                <td class="subTituloDireita" width="16%"> Liberação de Pedido: </td>
                <td id="td_libera_analise" colspan="4">
                    <input type="checkbox" name="ftprliberaparecerchefe" id="ftprliberaparecerchefe" value="S" onclick="atualizaStatusLiberaParecer(this);" <?=$checked?> >
                    <span style="position:relative; top:-4px; <?=$cor?>">Liberar pedido de Análise da Chefia</span>
                </td>
            </tr>
        </table>
    <?  } ?>
        
    <br>
    
    <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td colspan="4" class="subtituloEsquerda" width="15%" style="font-size: 11px; background-color: #e9e9e9;"> Manifestação Expressa do(a) Servidor(a) </td>
        </tr>
        <tr>
            <td colspan="4" class="textoJustificado">
                <p>
                    Por intermédio deste documento venho requerer ao meu Chefe Imediato ou ao seu Substituto, responsável pela minha Avaliação de Desempenho Individual referente à 
                    Gratificação de Desempenho do Plano Geral de Cargos do Poder Executivo - GDPGPE, RECONSIDERAÇÃO do resultado final obtido na Avaliação Individual, 
                    com base nas seguintes justificativas:
                    <br><br>
                    <span style="font-size: 10px">(Descrever o motivo da não concordância com a avaliação de desempenho, apontando por Fator de Avaliação)</span>
                </p>
            </td>
        </tr>
    </table>
    <br>
    <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="subtituloEsquerda" colspan="2">Conhecimento de métodos e técnicas para o desenvolvimento das atividades do cargo efetivo</td>
        </tr>
        <tr>
            <td colspan="2">
                <table align="left" style="width:98%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tr>
                        <td>Justificativa:<br>
                            <? echo campo_textarea('ftprconmettecdesatcaref', 'N', $habilitado, 'Justificativa', 200, 8, 4000, '','','','','','','99%'); ?>
                        </td>
                        <td valign="top">
                            <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                                <tr>
                                    <td class="subtitulocentro" colspan="2">Pontuação</td>
                                </tr>
                                <tr>
                                    <td class="subtitulocentro">Recebida</td>
                                    <td class="subtitulocentro">Solicitada</td>
                                </tr>
                                <tr style="text-align: center;">
                                    <td><?= campo_texto('ftprpontrecebida', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontrecebida"', 'validaValor(this);', $ftprpontrecebida ); ?></td>
                                    <td><?= campo_texto('ftprpontsolicitada', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontsolicitada"', 'validaValor(this);', $ftprpontsolicitada ); ?></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <?php
            if( $tp_gra != 'PS'){
        ?>
        <tr>
            <td class="subtituloEsquerda" colspan="2">Produtividade no trabalho</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width:98%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td>Justificativa:<br>
                        <? echo campo_textarea('ftprprodtrabalho', 'N', $habilitado, 'Justificativa', 200, 8, 4000, '','','','','','','99%'); ?></td>
                    <td valign="top">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=campo_texto('ftprpontrecebidaprodtrab', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontrecebidaprodtrab"', 'validaValor(this);', $ftprpontrecebidaprodtrab ); ?></td>
                                <td><?=campo_texto('ftprpontsolicprodtrab', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontsolicprodtrab"', 'validaValor(this);', $ftprpontsolicprodtrab ); ?></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        
        <tr>
            <td class="subtituloEsquerda" colspan="2">Capacidade de auto desenvolvimento</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width:98%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td>Justificativa:<br>
                        <? echo campo_textarea('ftprcapautdesen', 'N', $habilitado, 'Justificativa', 200, 8, 4000, '','','','','','','99%'); ?></td>
                    <td valign="top">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=campo_texto('ftprpontrecebcapdesen', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontrecebcapdesen"', 'validaValor(this);', $ftprpontrecebcapdesen ); ?></td>
                                <td><?=campo_texto('ftprpontsoliccapdesen', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontsoliccapdesen"', 'validaValor(this);', $ftprpontsoliccapdesen ); ?></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        
        <tr>
            <td class="subtituloEsquerda" colspan="2">Relacionamento interpessoal</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width:98%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td>Justificativa:<br>
                        <? echo campo_textarea('ftprrelacinterpessoal', 'N', $habilitado, 'Justificativa', 200, 8, 4000, '','','','','','','99%'); ?></td>
                    <td valign="top">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=campo_texto('ftprpontrecebrelintpes', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontrecebrelintpes"', 'validaValor(this);', $ftprpontrecebrelintpes ); ?></td>
                                <td><?=campo_texto('ftprpontsolicrelintpes', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontsolicrelintpes"', 'validaValor(this);', $ftprpontsolicrelintpes ); ?></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        <?php
            }
        ?>
        <tr>
            <td class="subtituloEsquerda" colspan="2">Trabalho em equipe</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width:98%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td>Justificativa:<br>
                        <? echo campo_textarea('ftprtrabequipe', 'N', $habilitado, 'Justificativa', 200, 8, 4000, '','','','','','','99%'); ?></td>
                    <td valign="top">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=campo_texto('ftprpontrecebtrabequipe', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontrecebtrabequipe"', 'validaValor(this);', $ftprpontrecebtrabequipe ); ?></td>
                                <td><?=campo_texto('ftprpontsolictrabequipe', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontsolictrabequipe"', 'validaValor(this);', $ftprpontsolictrabequipe ); ?></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda" colspan="2">Comprometimento com trabalho</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width:98%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td>Justificativa:<br>
                        <? echo campo_textarea('ftprcomprtrabalho', 'N', $habilitado, 'Justificativa', 200, 8, 4000, '','','','','','','99%'); ?></td>
                    <td valign="top">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=campo_texto('ftprpontrecebcomptrab', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontrecebcomptrab"', 'validaValor(this);', $ftprpontrecebcomptrab ); ?></td>
                                <td><?=campo_texto('ftprpontsoliccomptrab', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontsoliccomptrab"', 'validaValor(this);', $ftprpontsoliccomptrab ); ?></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda" colspan="2">Cumprimento das normas de procedimento de conduta no desempenho das atribuições do cargo</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width:98%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td>Justificativa:<br>
                        <? echo campo_textarea('ftprcumpnorproccodatrcarg', 'N', $habilitado, 'Justificativa', 200, 8, 4000, '','','','','','','99%'); ?></td>
                    <td valign="top">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=campo_texto('ftprpontrecebcumpnorproccodatrcarg', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontrecebcumpnorproccodatrcarg"', 'validaValor(this);', $ftprpontrecebcumpnorproccodatrcarg ); ?></td>
                                <td><?=campo_texto('ftprpontsoliccumpnorproccodatrcarg', 'N', $habilitado, '', 5, 3, '###', '', '', '', 0, 'id="ftprpontsoliccumpnorproccodatrcarg"', 'validaValor(this);', $ftprpontsoliccumpnorproccodatrcarg ); ?></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        <tr>
            <td colspan="2">Nestes termos, pede deferimento.</td>
        </tr>
        <tr>
            <td style="text-align: center; height: 100px" colspan="2">
            Local e Data: _______________, ____ de ____________ de <?=date('Y'); ?>.</td>
        </tr>
        <tr>
            <td style="text-align: center;" colspan="2">___________________________________<br> Assinatura do(a) Servidor(a) Avaliado(a)
            </td>
        </tr>
    </table>
    
    <br>
    <?PHP
        $tipo_usu = buscaPermissaoReconcideracao( $fdpcpf );

        if( ($tipo_usu['usuario_tipo'] == 'CH' || (in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil )) ) && ($ftprid != '' && $ftprliberaparecerchefe == 't') ){
    ?>
            <table align="center" border="0" width="95%" class="tabela listagem" cellpadding="3" cellspacing="1">
                <tr>
                    <td colspan="3" class="subTituloCentro" style="font-size: 12px; background-color: #dcdcdc;">
                        Reconsideração da Chefia Imediata ou do Substituto
                    </td>
                </tr>
                <tr>
                    <td colspan="3">Parecer:<br>
                    <? echo campo_textarea('flprparcerchefia', 'N', 'S', 'Parecer', 292, 6, 4000, '','','','','','','99%'); ?></td>
                </tr>
                <tr>
                    <td colspan="3">
                        <tr>
                            <td width="30%"><input type="radio" <?=(($ftprpedido == 'D') ? 'checked="checked"' : '') ?>  name="ftprpedido" id="ftprpedido" value="D"><span style="position:relative; top:-3px;"> Pedido Deferido </sapn></td>
                            <td width="30%"><input type="radio" <?=(($ftprpedido == 'P') ? 'checked="checked"' : '') ?>  name="ftprpedido" id="ftprpedido" value="P"><span style="position:relative; top:-3px;"> Pedido Deferido Parcialmente </sapn></td>
                            <td width="30%"><input type="radio" <?=(($ftprpedido == 'I') ? 'checked="checked"' : '') ?>  name="ftprpedido" id="ftprpedido" value="I"><span style="position:relative; top:-3px;"> Pedido Indeferido </sapn></td>
                        </tr>
                    </td>
                </tr>
                <tr>
                    <td style="text-align:center; height:50px" colspan="3">Local e Data: _______________, ____ de ____________ de <?=date('Y'); ?>.</td>
                </tr>
                <tr>
                    <td style="text-align: center;  height:50px" colspan="3">_____________________________________________<br> Assinatura e carimbo da Chefia Imediata ou Substituto</td>
                </tr>
            </table>

            
    <?PHP
        }
        
        if( ($tipo_usu['usuario_tipo'] == 'CH' || (in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil )) ) && ( $ftprid != '' && $ftprliberaparecerchefe == 't' && ($ftprpedido != '' && $ftprpedido != 'D') ) ){
    ?>
            <br>
            
            <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td colspan="3" class="subTituloCentro" style="font-size: 12px; background-color: #dcdcdc;">
                        Responsabilidade da Coordenação-Geral de Gestão de Pessoas
                    </td>
                </tr>
                <tr>
                    <td colspan="2">Ciência do(a) Avaliado(a) do resultado do Pedido de Reconsideração:</td>
                </tr>
                <tr>
                    <td colspan="2">
                    <table align="left" style="width: 100%" border="0" cellpadding="3" cellspacing="1">
                        <tr>
                            <td width="33.3%"><input type="radio" <?=(($ftprcienavaliador == 'CO') ? 'checked="checked"' : '') ?>  name="ftprcienavaliador" id="ftprcienavaliador" value="CO"><span style="position:relative; top:-3px;"> Concordo </sapn></td>
                            <td width="33.3%"><input type="radio" <?=(($ftprcienavaliador == 'DR') ? 'checked="checked"' : '') ?>  name="ftprcienavaliador" id="ftprcienavaliador" value="DR"><span style="position:relative; top:-3px;"> Discordo, informando que irei interpor Recurso. </sapn></td>
                            <td width="33.3%"><input type="radio" <?=(($ftprcienavaliador == 'DN') ? 'checked="checked"' : '') ?>  name="ftprcienavaliador" id="ftprcienavaliador" value="DN"><span style="position:relative; top:-3px;"> Discordo, porém não vou interpor Recurso. </sapn></td>
                        </tr>
                    </table></td>
                </tr>
                <tr>
                    <td style="text-align: center; height:50px;" colspan="2">Local e Data: _______________, ____ de ____________ de <?=date('Y'); ?>.</td>
                </tr>
                <tr>
                    <td style="text-align: center; height:50px;" colspan="2">_____________________________________________<br> Assinatura do(a) Servidor(a) Avaliado(a)<br><br></td>
                </tr>
            </table>
    <?  } ?>
            <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td colspan="2" class="subtituloCentro">
                        <input type="button" name="bt_gravar" value="Gravar Pedido" onclick="gravarPedido();">
                    </td>
                </tr>
                <tr>
                    <td colspan="2" class="subTituloDireita">
                        <img onclick="imprimir();" style="cursor: pointer;" src="../imagens/print.png">
                            <span onclick="imprimir();" style="cursor: pointer; color: green; font-style: italic; position: relative; top: -2px;" > Imprimir Pedido de Reconsidereção </span>
                        
                    </td>
                </tr>
            </table>
</form>