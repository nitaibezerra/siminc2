<?PHP
    
    header("Cache-Control: no-store, no-cache, must-revalidate");
    header("Cache-Control: post-check=0, pre-check=0", false);
    header("Pragma: no-cache");
    
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }
    
    $cpfavaliado = $_SESSION['cpfavaliado'];
    
    #VERIFICA SE EXISTE A SESSÃO. "USUARIO SELECIONADO", CASO NÃO VIRIFICA QUAL O PERFIL E REDIRECIONA PARA A RESPECTIVA PAGINA.
    $perfil = pegaPerfilGeral();
    
    $permissao = buscaPermissaoReconcideracao( $cpfavaliado );
    
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
    
    function buscarRecursoAvaliacao(){
        global $db;
        
        $cpfavaliado = $_SESSION['cpfavaliado'];
        
        $sql = "
            SELECT  ftrid, 
                    ftrargfundamentacao
            FROM gestaopessoa.ftrecavaliacaoindividual 
            
            WHERE fdpcpf = '{$cpfavaliado}' AND anoreferencia = {$_SESSION['exercicio']}
        ";
        $dados = $db->pegaLinha( $sql );
        
        return $dados;
    }
    
    function buscaDadosServidor(){
        global $db;
        
        $cpfavaliado = $_SESSION['cpfavaliado'];
        
        $sql = "
            SELECT  ser.sercpf, 
                    ser.sernome,
                    ser.sersiape,
                    ser.sercargo,
                    tls.tlssigla,
                    ser.sercpfchefe
            FROM gestaopessoa.servidor ser
            LEFT JOIN gestaopessoa.tipolotacaoservidor tls on tls.tlsid = ser.tlsid and tls.tlsstatus = 'A'
            
            WHERE ser.sercpf = '$cpfavaliado' AND ser.seranoreferencia = {$_SESSION['exercicio']}
        ";
            
        $dados = $db->pegaLinha($sql);
        
        return $dados;        
    }
        
    function salvarRecursoAvaliacao( $dados ){
        global $db;
        
        extract($dados);
        
        $cpfavaliado = $_SESSION['cpfavaliado'];
	
	$ftrargfundamentacao = addslashes($ftrargfundamentacao);
	
	if( $ftrid ){
            $sql = "
                UPDATE gestaopessoa.ftrecavaliacaoindividual
                    SET fdpcpf              = '{$cpfavaliado}',
                        anoreferencia       = '{$_SESSION['exercicio']}',
                        ftrargfundamentacao = '{$ftrargfundamentacao}',
                        ftrperiodoavalini   = '".DATA_PERIODO_INICO_AVALIACAO."',
                        ftrperiodoavalfim   = '".DATA_PERIODO_FINAL_AVALIACAO."'
                    WHERE ftrid = {$ftrid} RETURNING ftrid;
            ";
	} else {
            $sql = "
                INSERT INTO gestaopessoa.ftrecavaliacaoindividual(
                        fdpcpf, 
                        anoreferencia, 
                        ftrargfundamentacao, 
                        ftrperiodoavalini, 
                        ftrperiodoavalfim
                    )VALUES(
                        '{$cpfavaliado}', 
                        '{$_SESSION['exercicio']}', 
                        '{$ftrargfundamentacao}', 
                        '".DATA_PERIODO_INICO_AVALIACAO."',
                        '".DATA_PERIODO_FINAL_AVALIACAO."'
                    ) RETURNING ftrid;
            ";
	}
	$ftrid = $db->pegaUm($sql);
	
	if( $ftrid > 0 ){
            $db->commit();
            $db->sucesso( 'principal/avaliacao_servidor/cad_recurso_avaliacao_individual' );
	} else {
            $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/avaliacao_servidor/cad_recurso_avaliacao_individual');
	}
    }        
        

    include  APPRAIZ."includes/cabecalho.inc";
    echo '<br>';
    $db->cria_aba( $abacod_tela, $url, '' );
    echo '<br>';

    monta_titulo( 'Gestão de Pessoas', 'Formulário de Recurso de Avaliação Individual' );
    echo '<br>';

    #VARIAVEL E CONTROLE DE ABAS.
    $perfil = pegaPerfilGeral();
        
    if( in_array( PERFIL_SUPER_USER, $perfil ) || in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) ){
        $abacod_tela = ABA_CAD_AVALIACAO_SERVIDOR_ADM;
    }else{
        $abacod_tela = ABA_CAD_AVALIACAO_SERVIDOR;
    }
    
    $url            = 'gestaopessoa.php?modulo=principal/avaliacao_servidor/cad_recurso_avaliacao_individual&acao=A';
    $parametros     = '';
    
    $db->cria_aba($abacod_tela, $url, $parametros);
    
    $dados = buscarRecursoAvaliacao();
    
    $dadosServidor = buscaDadosServidor();
	
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

    <script type="text/javascript">

        function imprimir(){
            window.open("gestaopessoa.php?modulo=principal/avaliacao_servidor/imp_cad_recurso_avaliacao_individual&acao=A","Recurso Avaliação",'scrollbars=yes,height=600,width=1200,status=no,toolbar=no,menubar=no,location=no');
        }

        function salvarRecursoAvaliacao(){

            if( $('#ftrargfundamentacao').val() == '' ){
                alert('O campo "Argumentação/Fundamentação" é de preenchimento obrigatório!');
                $('#ftrargfundamentacao').focus();
                return false;
            }

            $('#requisicao').val('salvarRecursoAvaliacao');
            $('#formulario').submit();
        }

    </script>

    <form id="formulario" name="formulario" method="post" action="">
        <input type="hidden" name="requisicao" id="requisicao" value="">
        <input type="hidden" name="ftrid" id="ftrid" value="<?= $dados['ftrid']; ?>">

        <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td colspan="5" class="subTituloCentro" style="font-size: 15px; background-color: #dcdcdc;">
                    Formulário de Recurso de Avaliação Individual - Ciclo Avaliativo:  <?=DATA_PERIODO_INICO_AVALIACAO;?> a <?=DATA_PERIODO_FINAL_AVALIACAO;?>
                </td>
            </tr>
            <tr>
                <td colspan="5" class="subTituloCentro" style="font-size: 10px; background-color: #dcdcdc;">
                    Gratificação de Desempenho do Plano Geral de Cargos do Poder Executivo - Decreto nº 7.133, de 19 de março de 2010
                </td>
            </tr>
            <tr>
                <td colspan="2" class="subtituloEsquerda" width="45%" style="font-size: 11px; background-color: #e9e9e9;"> Identificação do(a) Servidor(a) </td>
            </tr>
            <tr>
                <td class="subTituloDireita"> Nome: </td>
                <td> <?=$dadosServidor['sernome'];?> </td>
            </tr>
            <tr>
                <td class="subTituloDireita"> SIAPE: </td>
                <td> <?=$dadosServidor['sersiape'];?> </td>
            </tr>
            <tr>
                <td class="subTituloDireita"> Cargo Efetivo: </td>
                <td> <?=$dadosServidor['sercargo'];?> </td>
            </tr>
            <tr>
                <td class="subTituloDireita"> Unidade de Exercício: </td>
                <td> <?=$dadosServidor['tlssigla'];?> </td>
            </tr>
            <tr>
                <td class="subTituloDireita"> Período de Avaliação: </td>
                <td> <b>Ciclo Avaliativo: 01/11/2012 a 31/10/2013</b> </td>
            </tr>
        </table>
        
        <br>
        
        <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td colspan="2" class="subtituloEsquerda" width="45%" style="font-size: 11px; background-color: #e9e9e9;"> Argumentação/Fundamentação </td>
            </tr>
            <tr>
                <td><? echo campo_textarea('ftrargfundamentacao', 'N', 'S', 'Argumentação/Fundamentação', 238, 8, 4000, '', 0, '', false, NULL, $dados['ftrargfundamentacao']); ?></td>
            </tr>
        </table>
        
        <br>
        
        <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
            <thead>
            <tr>
                <td class="subtitulocentro" width="50%">Local e Data</td>
                <td class="subtitulocentro" width="50%">Assinatura do(a) Servidor(a)</td>
            </tr>
            </thead>
            <tbody>
            <tr style="text-align: center;">
                <td width="50%" style="height: 70px;">_______________, ____ de ____________ de <?=date('Y'); ?>.</td>
                <td width="50%" style="height: 70px;">_____________________________________________</td>
            </tr>
            </tbody>
        </table>
        
        <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td colspan="2" class="subtituloCentro">
                    <input type="button" name="bt_gravar" value="Gravar Recurso" onclick="salvarRecursoAvaliacao();">
                </td>
            </tr>
            <tr>
                <td colspan="2" class="subTituloDireita">
                    <img onclick="imprimir();" style="cursor: pointer;" src="../imagens/print.png">
                        <span onclick="imprimir();" style="cursor: pointer; color: green; font-style: italic; position: relative; top: -2px;" > Imprimir Recurso </span>

                </td>
            </tr>
        </table>
    </form>
