<?PHP

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }
    
    function excluirDadosObjDemanda( $dados ){
        global $db;

        $sql = "
            DELETE FROM gestaodocumentos.objetonaoies WHERE oniid = {$dados['oniid']} RETURNING oniid;
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
    
    function buscaDadosObjDemanda($dados){
        global $db;
        
        $oniid = $dados['oniid'];

        $sql = "
            SELECT  oniid,
                    TRIM(onitipoobjdemanda) AS onitipoobjdemanda,
                    TRIM( oniidsc ) AS oniidsc, 
                    TRIM( CASE WHEN oniicnpj <> '' AND char_length(oniicnpj) = 14
                        THEN replace(to_char(cast(oniicnpj as bigint), '00:000:000/0000-00'), ':', '.') 
                        ELSE
                            CASE WHEN oniicnpj <> '' AND char_length(oniicnpj) = 11
                                THEN replace(to_char(cast(oniicnpj as bigint), '000:000:000-00'), ':', '.') 
                                ELSE ''
                            END
                    END ) AS oniicnpj, 
                    TRIM( oniitelefone ) AS oniitelefone, 
                    TRIM( oniiemail ) AS oniiemail
            FROM gestaodocumentos.objetonaoies 
            WHERE oniid = '{$oniid}';
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            $dados['oniidsc'] = iconv( "ISO-8859-1", "UTF-8", stripslashes( $dados["oniidsc"] ) );
            $dados['oniiemail'] = iconv( "ISO-8859-1", "UTF-8", stripslashes( $dados["oniiemail"] ) );
        }else{
            $dados = '';
        }        
        echo simec_json_encode( $dados );
        die();
    }
    
    function salvarDadosObjDemanda($dados){
        global $db;

        extract($dados);
        
        $oniicnpj = trim( str_replace( ".", "", str_replace("-", "", str_replace("/", "", $dados['oniicnpj']) ) ) );

        if( $ojdtipo == 'O' || $ojdtipo == 'N' || $ojdtipo == 'D' ){
            if($oniid == ''){
                $sql_obj = "
                    INSERT INTO gestaodocumentos.objetonaoies(
                                oniidsc, 
                                oniicnpj, 
                                oniitelefone, 
                                oniiemail,
                                onitipoobjdemanda
                        ) VALUES (
                                '{$oniidsc}',
                                '{$oniicnpj}',
                                '{$oniitelefone}',
                                '{$oniiemail}',
                                '{$ojdtipo}'
                        )RETURNING oniid;
                ";
            }else{
                $sql_obj = "
                    UPDATE gestaodocumentos.objetonaoies
                            SET oniidsc           = '{$oniidsc}',
                                oniitelefone      = '{$oniitelefone}', 
                                oniicnpj          = '{$oniicnpj}', 
                                oniiemail         = '{$oniiemail}',
                                onitipoobjdemanda = '{$ojdtipo}'
                    WHERE oniid = {$oniid} RETURNING oniid;
                ";
            }
            $oniid = $db->pegaUm($sql_obj);
        }

        if($oniid != '' && ( $ojdtipo == 'O' || $ojdtipo == 'N' || $ojdtipo == 'D' ) ){
            $_sql = "
                INSERT INTO gestaodocumentos.objetodemanda(
                            oniid, 
                            tarid, 
                            ojdtipo
                        ) VALUES (
                            {$oniid},
                            {$tarid},
                            '{$ojdtipo}'
                        )RETURNING ojdid;
            ";
            $ojdid = $db->pegaUm($_sql);
        }
        
        if($ojdtipo == 'I'){
            $_sql = "
                INSERT INTO gestaodocumentos.objetodemanda(
                            iesid, 
                            tarid, 
                            ojdtipo
                        ) VALUES (
                            {$iesid},
                            {$tarid},
                            '{$ojdtipo}'
                        )RETURNING ojdid;
            ";
            $ojdid = $db->pegaUm($_sql);
        }
        
        if($ojdtipo == 'M'){
            $_sql = "
                INSERT INTO gestaodocumentos.objetodemanda(
                            mntid, 
                            tarid, 
                            ojdtipo
                        ) VALUES (
                            {$mntid},
                            {$tarid},
                            '{$ojdtipo}'
                        )RETURNING ojdid;
            ";
            $ojdid = $db->pegaUm($_sql);
        }

        if($oniid != '' || $ojdid != ''){
            $db->commit();
            $db->sucesso("principal/cadTarefa", "&tarid={$tarid}", "Operação realizada com sucesso", "S", "S");
        }
        die();
    }
?>

<html>
    <title>Cadastrar Objeto de Demanda</title>
    <head>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        
    <script type="text/javascript">
        
        function abrirPopupInstituicao() {
            window.open('gestaodocumentos.php?modulo=principal/popupInstituicoes&acao=A', '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=820,height=560');
        }
        
        function abrirPopupMantenedora() {
            window.open('gestaodocumentos.php?modulo=principal/popupMantenedora&acao=A', '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=820,height=560');
        }        
        
        function buscaDadosObjDemanda( oniid ){ 
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=buscaDadosObjDemanda&oniid="+oniid,
                success: function(resp){
                    var dados = $.parseJSON(resp);
                    $('#oniid').val(dados.oniid);
                    $('#ojdtipo').val(dados.onitipoobjdemanda);
                    $('#oniicnpj').val(dados.oniicnpj);
                    $('#oniidsc').val(dados.oniidsc);
                    $('#oniitelefone').val(dados.oniitelefone);
                    $('#oniiemail').val(dados.oniiemail);
                }
            });
        }

        function excluirDadosObjDemanda(oniid){
            var confirma = confirm("Deseja realmente excluir o Registro?");
            if( confirma ){
                $.ajax({
                    type    : "POST",
                    url     : window.location,
                    data    : "requisicao=excluirDadosObjDemanda&oniid="+oniid,
                    asynchronous: false,
                    success: function(resp){
                        resp = pegaRetornoAjax('<resp>', '</resp>', resp, true);
                        if( trim(resp) == 'OK' ){
                            alert('Operação Realizada com sucesso!');
                            window.location.reload();
                        }else{
                            alert('Ocorreu um problema, não foi possivél realizar a operação. Este Objeto da Demanda deve esta sendo usado em alguma Demanda!');
                        }
                    }
                });
            }
        }

        function fecharPopup() {
            return window.close();
        }
 
        function validar_cpf_cnpj( oniicnpj ){
            var tamanho = $('#oniicnpj').val().length;
            var valido;

            if( tamanho == 14 ){
                valido = validar_cpf( oniicnpj );
                if(!valido){
                    alert( "CPF inválido! Favor informar um CPF válido!" );
                    $('#oniicnpj').focus();
                    return false;
                }else{
                    return true;
                }
            }else if( tamanho == 18 ){
                valido = validarCnpj( oniicnpj );
                if(!valido){
                    alert( "CNPJ inválido! Favor informar um CNPJ válido!" );
                    $('#oniicnpj').focus();
                    return false;
                }else{
                    return true;
                }
            }
        }
    
        function mascaraCPF_CNPJ( oniicnpj ){
            var tamanho = $('#oniicnpj').val().length;
            var cpf, cnpj;
        
            if( tamanho <= 14 ){
                cpf = mascaraglobal('###.###.###-##', oniicnpj);
                $('#oniicnpj').val(cpf);
            }else if( tamanho <= 18 ){
                cnpj = mascaraglobal('##.###.###/####-##', oniicnpj);
                $('#oniicnpj').val(cnpj);
            }else{
                alert('CPF/CNPJ Ivalido, verifique o número digitado!');
            }
        }

        function verificaTipoSolicitante(tipo) {
            if( tipo == 'O' || tipo == 'N' || tipo == 'D' ){
                $('#tr_oniidsc').css("display", "");
                $('#tr_oniicnpj'). css("display", "");
                $('#tr_oniitelefone').css("display", "");
                $('#tr_oniiemail').css("display", "");
                $('#tr_tabela_busca_obj').css("display", "");
                $('#div_tabela_busca_obj').css("display", "");
                $('#ver_todos').css("display", "");
                $('#buscar').css("display", "");                
            }else{
                $('#tr_oniidsc').css("display", "none");
                $('#tr_oniicnpj'). css("display", "none");
                $('#tr_oniitelefone').css("display", "none");
                $('#tr_oniiemail').css("display", "none");
                $('#tr_tabela_busca_obj').css("display", "none");
                $('#div_tabela_busca_obj').css("display", "none");
                $('#ver_todos').css("display", "none");
                $('#buscar').css("display", "none");
            }

            if(tipo == 'M'){
                $('#tr_buscar_mantenedora').css("display", "");
            }else{
                $('#tr_buscar_mantenedora').css("display", "none");
            }
            
            if(tipo == 'I'){
                $('#tr_buscar_instituicao').css("display", "");
            }else{
                $('#tr_buscar_instituicao').css("display", "none");
            }
        }

        function excluirSolicitante( solid ){
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
                            window.location.reload();
                        }else{
                            alert('Ocorreu um problema, não foi possivél realizar a operação. Este solicitante deve esta sendo usado em alguma Demanda!');
                        }
                    }
                });
            }
        }

        function enviaForm(){
            var ojdtipo     = $('#ojdtipo');
            var oniicnpj    = $('#oniicnpj');
            var oniidsc     = $('#oniidsc');
            
            var erro;
            
            if(!ojdtipo.val()){
                alert('O campo "Tipo de Objeto de Demanda" é um campo obrigatório!');
                ojdtipo.focus();
                erro = 1;
                return false;
            }            
            
            if(ojdtipo.val() == 'O' ){
                if(!oniidsc.val()){
                    alert('O campo "Razão Social/Nome" é um campo obrigatório!');
                    oniidsc.focus();
                    erro = 1;
                    return false;
                }
            }

            if(ojdtipo.val() == 'I' ){
                if($('#div_instituicao').html() == ''){
                    alert('É necessário adicionar uma Instituição.');
                    erro = 1;
                    return false;
                }
            }

            if(ojdtipo.val() == 'M' ){
                if($('#div_mantenedora').html() == ''){
                    alert('É necessário adicionar uma Mantenedora.');
                    erro = 1;
                    return false;
                }
            }
            
            if(!erro){
                $('#requisicao').val('salvarDadosObjDemanda');
                $('#formulario').submit();
            }
        }
        
        function pesquisarObjtDemanda(param){
            if(trim(param) == 'fil'){
                $('#tipo_fil').val('fil');
                $('#formulario').submit();
            }else{
                $('#tipo_fil').val('');
                $('#formulario').submit();
            }
        }
    </script>
        
    </head>
    <br>    
    <form name="formulario" id="formulario" method="post">
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <input type="hidden" name="requisicao" id="requisicao" value="" />
            <input type="hidden" name="oniid" id="oniid" value="" />
            <input type="hidden" name="tipo_fil" id="tipo_fil" value="" />
            <input type="hidden" name="tarid" id="tarid" value="<?php echo $_GET['tarid']; ?>" />
            <tr>
                <td class = "subtitulodireita" colspan="2"><center> <h3>Cadastro de Objeto da Demanda</h3></center></td> 
            </tr>
            <tr>
                <td class = "subtitulodireita" width="30%">Tipo de Objeto da Demanda:</td>
                <td>
                    <?PHP
                        $sql = array(
                            array("codigo"=>"M", "descricao"=>"Mantenedora"),
                            array("codigo"=>"I", "descricao"=>"IES"),
                            array("codigo"=>"N", "descricao"=>"Não IES"),
                            array("codigo"=>"D", "descricao"=>"IES Descredenciada"),
                            array("codigo"=>"O", "descricao"=>"Outros" )
                        );
                        $db->monta_combo("ojdtipo", $sql, 'S', 'Selecione...', 'verificaTipoSolicitante', '', '', '415', 'S', 'ojdtipo', null, 'O', 'Tipo de Objeto da Demanda');
                    ?>
                </td>
            </tr>
            <tr id="tr_oniidsc">
                <td class = "subtitulodireita">Nome/Razão Social:</td>
                <td>
                    <?PHP 
                        echo campo_texto('oniidsc', 'S', 'S', '', 56, 50, '', '', '', '', '', 'id="oniidsc"', '', $oniidsc); 
                    ?>
                </td>
            </tr>
            <tr id="tr_oniicnpj">
                <td class = "subtitulodireita">CNPJ/CPF:</td>
                <td>
                    <?PHP 
                        echo campo_texto('oniicnpj', 'N', 'S', 'CNPJ', 56, 18, '', '', '', '', '', 'id="oniicnpj"', 'mascaraCPF_CNPJ(this.value)', $oniicnpj, 'validar_cpf_cnpj(this.value);'); 
                    ?>
                </td>
            </tr>            
            <tr id="tr_oniitelefone">
                <td class = "subtitulodireita">Telefone:</td>
                <td>
                    <?PHP
                        echo campo_texto('oniitelefone', 'N', 'S', 'Telefone', 56, 30, '##-####-####', '', '', '', '', 'id="oniitelefone"', '', $oniitelefone); 
                    ?>
                </td>
            </tr>
            <tr id="tr_oniiemail">
                <td class = "subtitulodireita">Email de Contato:</td>
                <td>
                    <?PHP
                        echo campo_texto('oniiemail', 'N', 'S', 'E-mail de Contato', 56, 30, '', '', '', '', '', 'id="oniiemail"', '', $oniiemail); 
                    ?>
                </td>
            </tr>
            
            <tr id="tr_buscar_instituicao" style="display: none;">
                <td class = "subtitulodireita">Buscar Instituição:</td>
                <td>
                    <input type="button" name="buscar_inst" id="buscar_inst" onclick="abrirPopupInstituicao();" value="Buscar Instituição">
                    <div id="div_instituicao" >
                        <?PHP
                            $resp = "<span style=\"color: #1874CD;\"\">".$rs[0]['iesid']." - ".$rs[0]['iessigla']." - ".$rs[0]['iesdsc']."</span>";
                            $resp .= "<input type=\"hidden\" name=\"iesid\" id=\"iesid\" value=\"{$rs[0]['iesid']}\" />";
                        ?>
                    </div>
                </td>	
            </tr>
            
            <tr id="tr_buscar_mantenedora" style="display: none;">
                <td class = "subtitulodireita">Buscar Mantenedora:</td>
                <td>
                    <input type="button" name="buscar_mantenedora" id="buscar_mantenedora" onclick="abrirPopupMantenedora('radio');" value="Buscar Mantenedora">
                    <div id="div_mantenedora" >
                        <?PHP
                            $resp = "<span style=\"color: #1874CD;\"\">".$rs[0]['mntid']." - ".$rs[0]['mntcnpj']." - ".$rs[0]['mntdsc']."</span>";
                            $resp .= "<input type=\"hidden\" name=\"mntid\" id=\"mntid\" value=\"{$rs[0]['mntid']}\" />";
                        ?>
                    </div>
                </td>	
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisarObjtDemanda('fil');">  
                    <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisarObjtDemanda('tudo');">  
                    <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="enviaForm();">  
                    <input type="button" name="btnFechar" value="Cancelar" id="btnFechar" onclick="fecharPopup();">  
                </td>
            </tr>
        </table>
    </form>
        <br>
        <div id="div_tabela_busca_obj">
            <?PHP
                if( $_REQUEST['tipo_fil'] == 'fil' ){
                    if ($_REQUEST['oniicnpj']) {
                        $oniicnpj = trim( str_replace( ".", "", str_replace("-", "", str_replace("/", "", $_REQUEST['oniicnpj']) ) ) );
                        $where = " AND ob.oniicnpj = '{$oniicnpj}'";
                    }

                    if ($_REQUEST['oniidsc']) {
                        $oniidsc = removeAcentos($_REQUEST['oniidsc']);
                        $where .= " AND public.removeacento(ob.oniidsc) ilike ('%{$oniidsc}%') ";
                    }
                }else{
                    $where = "";
                }
                
                $acao = "
                    <center>
                        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscaDadosObjDemanda('||ob.oniid||');\" title=\"Editar Objeeto Demanda\" >
                        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDadosObjDemanda('||ob.oniid||');\" title=\"Excluir Objeeto Demanda\" >
                    </center>        
                ";

                $sql = "
                    SELECT  '{$acao}' as acao,
                            ob.oniidsc, 
                            CASE WHEN ob.oniicnpj <> ''
                                THEN 
                                    CASE WHEN CHAR_LENGTH(ob.oniicnpj) = 11 
                                        THEN trim( replace( to_char( cast(ob.oniicnpj as bigint), '000:000:000-00' ), ':', '.' ) )
                                        ELSE trim( replace( to_char( cast(ob.oniicnpj as bigint), '00:000:000/0000-00' ), ':', '.' ) )
                                    END 
                                ELSE ''
                            END AS oniicnpj,
                            ob.oniitelefone, 
                            ob.oniiemail,
                            CASE 
                                WHEN onitipoobjdemanda = 'O' THEN 'OUTROS'
                                WHEN onitipoobjdemanda = 'N' THEN 'NÃO IES'
                                WHEN onitipoobjdemanda = 'D' THEN 'IES DESCREDEMCIADO'
                            END AS onitipoobjdemanda

                    FROM gestaodocumentos.objetonaoies ob

                    WHERE 1=1 {$where} 

                    ORDER BY ob.oniid
                ";
                $cabecalho = array("Ação", "Descrição", "CPF/CNPJ", "Telefone", "E-mail", "Tipo Obj. Demanda");  
                $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'left');
                //$tamanho = Array('5%', '10%', '50%', '10%', '10%', '10%');
                $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);

            ?>
        </div>