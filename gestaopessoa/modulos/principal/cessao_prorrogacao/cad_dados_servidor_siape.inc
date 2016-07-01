<?PHP
    if($_REQUEST['requisicao']) {        
        $_REQUEST['requisicao']($_REQUEST);
    }
    
    $cprid = $_GET['cprid'];
    $buscar = $_REQUEST['busca'];
    
    if( $buscar == 'S' ){
        $habilita = 'N';
    }else{
        $habilita = 'S';
    }

    function addServidorFormPai($dados){
        global $db;
        
        if( $_SESSION['gestao']['Aba'] == 'CESSAO' ){
            $url_p = 'cad_dados_cessao';
        }elseif( $_SESSION['gestao']['Aba'] == 'PRORROGACAO' ){
            $url_p = 'cad_dados_prorrogacao';
        }elseif( $_SESSION['gestao']['Aba'] == 'CONSULTA' ){
            $url_p = 'cad_dados_consulta';
        }elseif( $_SESSION['gestao']['Aba'] == 'RETIFICACAO' ){
            $url_p = 'cad_dados_retificacao';
        }
        if( $dados['siape'] != '' ){
            $db->sucesso('principal/cessao_prorrogacao/'.$url_p, '&cprid='.$cprid.'&siape='.$dados['siape'], 'Operação realizada com sucesso!', 'S', 'S');
        }
        die();
    }    
    
    function buscaDadosServidor($dados){
        global $db;
        
        $tipo  = $dados['tipo'];
        $param = str_replace(".", "", str_replace("-", "", $dados['param']));
        
        if($tipo == 'SIAPE'){
            $where = "WHERE nu_matricula_siape = '{$param}'";
        }else{
            $where = "WHERE nu_cpf = '{$param}'";
        }

        $sql = "
            SELECT  nu_matricula_siape,
                    nu_cpf,
                    co_orgao,
                    co_situacao_servidor,
                    co_funcao,
                    co_cargo_emprego  AS CARGO
            FROM siape.tb_servidor_simec {$where};
        ";
        $dados = $db->pegaLinha($sql);
        
        if($dados != ''){
            echo '<resp>OK</resp>';
        }else{
            echo '<resp>ERRO</resp>';
        }
        die;
    }
    
    function excluirSolicitante( $dados ){
        global $db;

        $sql = "
            DELETE FROM gestaodocumentos.solicitantepessoa WHERE solid = {$dados['solid']} RETURNING solid;
        ";
        $solid = $db->pegaUm($sql);

        if($solid > 0){
            $db->commit();
            echo '<resp>';
            echo 'OK';
            echo '</resp>';
        }
        die();
    }
    
    function salvarDadosServidoSiape($dados){
        global $db;
        
        extract($dados);
        
        $nu_cpf = trim( str_replace( ".", "", str_replace("-", "", $dados['nu_cpf'] ) ) );
        
        if( $nu_matricula_siape != '' ){
            $sql = "
                INSERT INTO siape.tb_servidor_simec(
                        nu_matricula_siape, 
                        nu_cpf, 
                        no_servidor, 
                        co_orgao, 
                        co_situacao_servidor, 
                        co_funcao, 
                        co_cargo_emprego
                    ) VALUES (
                        '{$nu_matricula_siape}',
                        '{$nu_cpf}',
                        '{$no_servidor}',
                        '{$co_orgao}',
                        '{$co_situacao_servidor}',
                        '{$co_funcao}',
                        '{$co_cargo_emprego}'
                    )RETURNING nu_matricula_siape;
            ";
            $nu_matricula_siape = $db->pegaUm($sql);
        }
        
        if( $_SESSION['gestao']['Aba'] == 'CESSAO' ){
            $url_p = 'cad_dados_cessao';
        }elseif( $_SESSION['gestao']['Aba'] == 'PRORROGACAO' ){
            $url_p = 'cad_dados_prorrogacao';
        }elseif( $_SESSION['gestao']['Aba'] == 'CONSULTA' ){
            $url_p = 'cad_dados_consulta';
        }elseif( $_SESSION['gestao']['Aba'] == 'RETIFICACAO' ){
            $url_p = 'cad_dados_retificacao';
        }
        
        if( $nu_matricula_siape > 0 ){
            $db->commit();
            $db->sucesso('principal/cessao_prorrogacao/'.$url_p, '&cprid='.$cprid.'&siape='.$nu_matricula_siape, 'Os dados foram gravados com sucesso!', 'S', 'S');
        }
        die();
    }

?>

<html>
    <title>Cadastrar Solicitante</title>
    <head>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        
    <script type="text/javascript">
        
        function addServidorFormPai( nu_matricula_siape ){
            $('#siape').val(nu_matricula_siape);
            $('#requisicao').val('addServidorFormPai');
            $('#formulario').submit();
        }
                
        function buscaDadosServidor( param, tipo ){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=buscaDadosServidor&param="+param+'&tipo='+tipo,
                success: function(resp){
                    resp = pegaRetornoAjax('<resp>', '</resp>', resp, true);
                    if( trim(resp) == 'OK' ){
                        $('#nu_matricula_siape').attr('disabled', true);
                        $('#nu_cpf').attr('disabled', true);
                        $('#no_servidor').attr('disabled', true);
                        $('#co_orgao').attr('disabled', true);
                        $('#co_situacao_servidor').attr('disabled', true);
                        $('#co_funcao').attr('disabled', true);
                        $('#co_cargo_emprego').attr('disabled', true);
                        $('#btnSalvar').css("display", "none");
                        $('#btnCancelar').css("display", "none");
                        $('#btnFechar').css("display", "");
                        
                        alert('Servidor já cadastrado, não é necessário o seu cadastramento!');
                        
                        window.close();
                    }
                }
            });
        }
        
        function excluirSolicitante(solid){
            var confirma = confirm("Deseja realmente excluir o Registro?");
            if( confirma ){
                $.ajax({
                    type    : "POST",
                    url     : window.location,
                    data    : "requisicao=excluirSolicitante&solid="+solid,
                    asynchronous: false,
                    success: function(resp){
                        resp = pegaRetornoAjax('<resp>', '</resp>', resp, true);
                        if( trim(resp) == 'OK' ){
                            alert('Operação Realizada com sucesso!');
                            //window.location.reload();
                            $('#formulario').submit();
                        }else{
                            alert('Ocorreu um problema, não foi possivél realizar a operação. Este solicitante deve esta sendo usado em alguma Demanda!');
                        }
                    }
                });
            }
        }
        
        function fecharPopup() {
            window.close();
        }
        
        function validarCpf( nu_cpf ){
            var valido;
            
            if(nu_cpf != ''){
                valido = validar_cpf( nu_cpf );
                if(!valido){
                    alert( "CPF inválido! Favor informar um CPF válido!" );
                    $('#nu_cpf').focus();
                    return false;
                }else{
                    return true;
                }
            }
        }

        function salvarDadosServidoSiape(){
            var nu_matricula_siape      = $('#nu_matricula_siape');
            var nu_cpf                  = $('#nu_cpf');
            var no_servidor             = $('#no_servidor');
            var co_orgao                = $('#co_orgao');
            var co_situacao_servidor    = $('#co_situacao_servidor');
            var co_funcao               = $('#co_funcao');
            var co_cargo_emprego        = $('#co_cargo_emprego');
            
            var erro;
            
            if(!nu_matricula_siape.val()){
                alert('O campo "Código SIAPE" é um campo obrigatório!');
                nu_matricula_siape.focus();
                erro = 1;
                return false;
            }            
            
            if(!nu_cpf.val()){
                alert('O campo "CPF" é um campo obrigatório!');
                nu_cpf.focus();
                erro = 1;
                return false;
            }

            if(!no_servidor.val()){
                alert('O campo "Nome do Servidor" é um campo obrigatório!');
                no_servidor.focus();
                erro = 1;
                return false;
            }
            
            if(!co_orgao.val()){
                alert('O campo "Orgão/Sigla" é um campo obrigatório!');
                co_orgao.focus();
                erro = 1;
                return false;
            }
            
            if(!co_situacao_servidor.val()){
                alert('O campo "Situação" é um campo obrigatório!');
                co_situacao_servidor.focus();
                erro = 1;
                return false;
            }
            
            if(!co_funcao.val()){
                alert('O campo "Função" é um campo obrigatório!');
                co_funcao.focus();
                erro = 1;
                return false;
            }
            
            if(!co_cargo_emprego.val()){
                alert('O campo "Cargo" é um campo obrigatório!');
                co_cargo_emprego.focus();
                erro = 1;
                return false;
            }
            
            if(!erro){
                $('#requisicao').val('salvarDadosServidoSiape');
                $('#formulario').submit();
            }
        }
        
    </script>
        
    </head>
    <br>    
    <form name="formulario" id="formulario" action="" method="POST">
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <input type="hidden" name="requisicao" id="requisicao" value="" />
            <input type="hidden" name="siape" id="siape" value="" />
            <tr>
                <td class = "subtitulodireita" colspan="2"><center> <h3>Cadastro Auxiliar de SIAPE Servidor</h3></center></td> 
            </tr>
            <tr>
                <td class = "subtitulodireita" width="30%">Códgio SIAPE:</td>
                <td>
                    <?PHP
                        echo campo_texto('nu_matricula_siape', 'S', $habilita, 'Códgio SIAPE', 57, 7, '#######', '', '', '', '', 'id="nu_matricula_siape"', '', $nu_matricula_siape, "buscaDadosServidor(this.value, 'SIAPE');"); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class = "subtitulodireita">CPF:</td>
                <td>
                    <?PHP 
                        echo campo_texto('nu_cpf', 'S', $habilita, 'CPF', 57, 18, '###.###.###-##', '', '', '', '', 'id="nu_cpf"', '', $nu_cpf, "buscaDadosServidor(this.value, 'CPF');validarCpf(this.value); "); 
                    ?> 
                </td>
            </tr>
            <tr>
                <td class = "subtitulodireita">Nome do Servidor:</td>
                <td>
                    <?PHP 
                        echo campo_texto('no_servidor', 'S', $habilita, '', 57, 50, '', '', '', '', '', 'id="no_servidor"', '', $no_servidor); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class = "subtitulodireita">Orgão/Sigla:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  co_orgao as codigo, 
                                    sg_orgao||' - '||ds_orgao as descricao 
                            FROM siape.tb_simec_orgao
                            ORDER BY ds_orgao asc
                        ";
                        $db->monta_combo("co_orgao", $sql, $habilita, 'Selecione...', '', '', '', '420', 'S', 'co_orgao', false, $co_orgao, 'Orgão/Sigla');
                    ?>					
                </td>
            </tr>
            <tr>
                <td class = "subtitulodireita">Situação:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  co_situacao_servidor as codigo, 
                                    ds_situacao_servidor as descricao 
                            FROM siape.tb_simec_situacao_servidor
                            ORDER BY ds_situacao_servidor asc
                        ";
                        $db->monta_combo("co_situacao_servidor", $sql, $habilita, 'Selecione...', '', '', '', '420', 'S', 'co_situacao_servidor', false, $co_situacao_servidor, 'Situação');
                    ?>	
                </td>	
            </tr>
            <tr>
                <td class = "subtitulodireita">Função:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  co_funcao as codigo, 
                                    ds_funcao as descricao 
                            FROM siape.tb_simec_funcao
                            ORDER BY ds_funcao asc
                        ";
                        $db->monta_combo("co_funcao", $sql, $habilita, 'Selecione...', '', '', '', '420', 'S', 'co_funcao', false, $co_funcao, 'Função');
                    ?>	
                </td>	
            </tr>
            <tr>
                <td class = "subtitulodireita">Cargo:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  co_cargo_emprego as codigo, 
                                    ds_cargo_emprego as descricao 
                            FROM siape.tb_simec_cargo_emprego
                            WHERE nu_jornada_trabalho <> 0
                            ORDER BY ds_cargo_emprego asc
                        ";
                        $db->monta_combo("co_cargo_emprego", $sql, $habilita, 'Selecione...', '', '', '', '420', 'S', 'co_cargo_emprego', false, $co_cargo_emprego, 'Cargo');
                    ?>	
                </td>	
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <? if($buscar != 'S'){ ?>
                        <input type="button" name="btnSalvar" id="btnSalvar" value="Salvar" onclick="salvarDadosServidoSiape();">
                        <input type="button" name="btnCancelar" id="btnCancelar" value="Cancelar" onclick="fecharPopup();">
                        <input type="button" name="btnFechar" id="btnFechar" value="Fechar" onclick="javascript:window.close();" style="display: none;">
                    <? } ?>
                </td>
            </tr>
        </table>
    </form>
    <br>
    <div id="div_tabela_busca_solicitante">
        <?PHP
            if( $buscar == 'S' ){
                if ($_REQUEST['nu_cpf']) {
                    $nu_cpf = trim( str_replace( ".", "", str_replace("-", "", str_replace("/", "", $_REQUEST['nu_cpf']) ) ) );
                    $where = " AND s.nu_cpf = '{$nu_cpf}'";
                }
            
            $acao = "
                <center>
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"addServidorFormPai('||s.nu_matricula_siape||');\" title=\"Selecionar Servidor\" >
                </center>        
            ";

            $sql = "
                SELECT  '{$acao}' as acao,
                        s.no_servidor,
                        ''||s.nu_matricula_siape,
                        trim( replace( to_char( cast(s.nu_cpf as bigint), '000:000:000-00' ), ':', '.' ) ) AS nu_cpf,
                        to_char(s.dt_ocorr_ing_orgao_serv, 'DD/MM/YYYY') as dt_ocorr_ing_orgao_serv,
                        co_cargo_emprego

                FROM siape.tb_servidor_simec s

                WHERE 1=1 {$where} 

                ORDER BY s.nu_matricula_siape
            ";
            $cabecalho = array("Ação", "Nom do Servidor", "SIAPE", "CPF", "Data de ingrasso", "Cargo");  
            $alinhamento = Array('center', 'left', 'left', 'left', 'left');
            //$tamanho = Array('5%', '10%', '50%', '10%', '10%', '10%');
            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
            
            }
        ?>
    </div>