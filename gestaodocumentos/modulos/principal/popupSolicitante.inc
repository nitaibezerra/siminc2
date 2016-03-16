<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    function pesquisarSolicitante(){
        global $db;
        header('content-type: text/html; charset=ISO-8859-1');

        extract($_POST);

    	if(trim($tipo_filtro) == 'filtro'){
            if($soltipopessoa){
                    $aryWhere[] = "s.soltipopessoa = '{$soltipopessoa}'";
            }

            if($solcpfcnpj){
                    $solcpfcnpj = trim( str_replace( ".", "", str_replace("-", "", str_replace("/", "", $solcpfcnpj) ) ) );
                $aryWhere[] = "s.solcpfcnpj = '{$solcpfcnpj}'";
            }

            if($solnome){
                $solnome = removeAcentos($solnome);
                $aryWhere[] = "public.removeacento(s.solnome) ILIKE ('%{$solnome}%')";
            }

            if($solemail){
                $aryWhere[] = "s.solemail ILIKE ('%{$solemail}%')";
            }

    	    if($estuf){
                $aryWhere[] = "s.estuf = '{$estuf}'";
            }

    	    if($muncod){
                $aryWhere[] = "s.muncod = '{$muncod}'";
            }
    	}

        $sql = "
            SELECT  '<img align=\"absmiddle\" src=\"/imagens/gif_inclui.gif\" style=\"cursor: pointer\" onclick=\"incluirSolicitante('||s.solid||');\" title=\"Incluir Solicitante\">
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscaDadosSolicitante('||s.solid||');\" title=\"Editar Solicitante\">
                    <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirSolicitante('||s.solid||');\" title=\"Excluir Solicitante\" >' AS acao,
                    
	            s.solnome,
                    
                    CASE WHEN s.solcpfcnpj <> ''
                        THEN
                        CASE WHEN CHAR_LENGTH(s.solcpfcnpj) = 11
                                THEN trim( replace( to_char( cast(s.solcpfcnpj as bigint), '000:000:000-00' ), ':', '.' ) )
                            ELSE trim( replace( to_char( cast(s.solcpfcnpj as bigint), '00:000:000/0000-00' ), ':', '.' ) )
                            END
                        ELSE ''
                    END AS solcpfcnpj,
                    
                    s.soltelefone,
                    s.solemail
            FROM gestaodocumentos.solicitantepessoa s
            
	    ".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
	    ORDER BY s.solid
        ";
	$cabecalho = array("Ação", "Descrição", "CPF/CNPJ", "Telefone", "E-mail");
        $alinhamento = Array('center', 'left', 'left', 'left', 'left');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', '', $alinhamento);
        die();
    }

    //Atualizar Combo Município
    function atualizaComboMun($dados){
        global $db;
        header('content-type: text/html; charset=ISO-8859-1');

        $sql = "SELECT  	muncod as codigo,
                    		mundescricao as descricao
            	FROM 		territorios.municipio
            	WHERE 		estuf = '{$dados['estuf']}'
            	ORDER BY 	mundescricao asc";

        $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', '420', 'N', 'muncod', false, null, 'Município');
        die();
    }

    function buscaDadosSolicitante($dados){
        global $db;
        $solid = $dados['solid'];

        $sql = "SELECT * FROM gestaodocumentos.solicitantepessoa WHERE solid = {$solid}";
        $dados = $db->pegaLinha($sql);

        $dados["solnome"] = iconv("ISO-8859-1", "UTF-8", $dados["solnome"]);
        $dados["solendereco"] = iconv("ISO-8859-1", "UTF-8", $dados["solendereco"]);
        $dados["solemail"] = iconv("ISO-8859-1", "UTF-8", $dados["solemail"]);

        echo simec_json_encode($dados);
        die();
    }

    function excluirSolicitante($dados){
        global $db;

        $sql = "DELETE FROM gestaodocumentos.solicitantepessoa WHERE solid = {$dados['solid']} RETURNING solid";
        $solid = $db->pegaUm($sql);

        if($solid > 0){
            $db->commit();
            echo 'S';
        } else {
        	echo 'N';
        }
        die();
    }

    function salvarDadosSolicitante($dados){
        global $db;

        extract($dados);
        $arrDados = explode('@', $dados['tpsid']);
        $tpsid    = trim(str_replace('tpsid_', '', $arrDados[0]));
        $tpstipo  = trim(str_replace('tpstipo_', '', $arrDados[1]));

        $solcpfcnpj = trim( str_replace( ".", "", str_replace("-", "", str_replace("/", "", $dados['solcpfcnpj']) ) ) );

        if (empty($muncod)) {
            $muncod = 'null';
        } else {
            $muncod = "'{$muncod}'";
        }

        if($tpstipo == 1){
            if($solid == ''){
                $sql_sol = "
                    INSERT INTO gestaodocumentos.solicitantepessoa(
                                soltipopessoa,
                                solcpfcnpj,
                                solnome,
                                soltelefone,
                                solemail,
                                solendereco,
                                solcep,
                                estuf,
                                muncod
                        ) VALUES (
                                '{$soltipopessoa}',
                                '{$solcpfcnpj}',
                                '{$solnome}',
                                '{$soltelefone}',
                                '{$solemail}',
                                '{$solendereco}',
                                '{$solcep}',
                                '{$estuf}',
                                $muncod
                        )RETURNING solid
                ";
                $solid = $db->pegaUm($sql_sol);
            } else {
                $sql_sol = "
                    UPDATE  gestaodocumentos.solicitantepessoa
                            SET soltipopessoa   = '{$soltipopessoa}',
		                solnome         = '{$solnome}',
		                solcpfcnpj      = '{$solcpfcnpj}',
		                soltelefone     = '{$soltelefone}',
		                solemail        = '{$solemail}',
		                solendereco     = '{$solendereco}',
		                solcep          = '{$solcep}',
		                estuf           = '{$estuf}',
		                muncod          = $muncod
                    WHERE solid = {$solid} RETURNING solid
                ";
               $solid = $db->pegaUm($sql_sol);
            }

            $sql = "SELECT solidpessoafisica FROM gestaodocumentos.instituicaosolicitante WHERE solidpessoafisica = {$solid} AND tarid = {$tarid}";
            $solicitante = $db->pegaUm($sql);

            if( empty($solicitante) ){
            	$_sql = "
                    INSERT INTO gestaodocumentos.instituicaosolicitante(
                            solidpessoafisica,
		            tarid,
		            tpsid
                        ) VALUES (
                            {$solid},
                            {$tarid},
                            {$tpsid}
                        )RETURNING issid
                    ";
		    $issid = $db->pegaUm($_sql);
            }
        }

        switch ($tpstipo) {
            case 2:
                if ($iesid) {
                    $_sql = "
                        INSERT INTO gestaodocumentos.instituicaosolicitante(
                                iesidinstituicaoensino,
                                tarid,
                                tpsid
                            ) VALUES (
                                {$iesid},
                                {$tarid},
                                {$tpsid}
                            )RETURNING issid
                    ";
                    $issid = $db->pegaUm($_sql);
                }
                break;
            case 3:
                if ($ogsid) {
                    $_sql = "
                        INSERT INTO gestaodocumentos.instituicaosolicitante(
                                ogsid,
                                tarid,
                                tpsid
                            ) VALUES (
                                {$ogsid},
                                {$tarid},
                                {$tpsid}
                            )RETURNING issid
                    ";
                    $issid = $db->pegaUm($_sql);
                }
                break;
            case 4:
                if ($uamid) {
                    $_sql = "
                        INSERT INTO gestaodocumentos.instituicaosolicitante(
                                uamid,
                                tarid,
                                tpsid
                             ) VALUES (
                                {$uamid},
                                {$tarid},
                                {$tpsid}
                             )RETURNING issid
                    ";
                    $issid = $db->pegaUm($_sql);
                }
                break;
            case 5:
                if ($mntid) {
                    $_sql = "
                        INSERT INTO gestaodocumentos.instituicaosolicitante(
                                mntid,
                                tarid,
                                tpsid
                            ) VALUES (
                                {$mntid},
                                {$tarid},
                                {$tpsid}
                            )RETURNING issid
                    ";
                    $issid = $db->pegaUm($_sql);
                }
                break;
        }

        if($solid != '' || $issid != ''){
            $db->commit();
            $db->sucesso('principal/cadTarefa', '&tarid='.$tarid, 'Operação Realizada com Sucesso!', 'S', 'S');
        }
    }

    function incluirSolicitante(){
        global $db;

        extract($_POST);

        $sql = "SELECT solidpessoafisica FROM gestaodocumentos.instituicaosolicitante WHERE solidpessoafisica = {$solid} AND tarid = {$tarid}";
        $solicitante = $db->pegaUm($sql);

        if(empty($solicitante)){
            $_sql = "
                INSERT INTO gestaodocumentos.instituicaosolicitante(
                        solidpessoafisica,
                        tarid,
                        tpsid
                    ) VALUES (
                        {$solidpessoafisica},
                        {$solid},
                        {$tarid},
                        1
                    ) RETURNING issid
            ";
            $issid = $db->pegaUm($_sql);
            $db->commit();
        }

        if($issid){
            echo 'S';
        } else {
            echo 'N';
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
        function abrirPopupInstituicao() {
            window.open('gestaodocumentos.php?modulo=principal/popupInstituicoes&acao=A', '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=820,height=560');
        }

        function abrirPopupOrgao() {
            var tpsid = $('#tpsid').val();
            var array = tpsid.split('@');
            tpsid = trim(array[0].replace('tpsid_', ''));
            window.open('gestaodocumentos.php?modulo=principal/popupOrgao&acao=A&tpsid='+tpsid, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=820,height=560');
        }

        function abrirPopupMantenedora() {
            window.open('gestaodocumentos.php?modulo=principal/popupMantenedora&acao=A', '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=820,height=560');
        }

        function atualizaComboMun( estuf ){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=atualizaComboMun&estuf="+estuf,
                async: false,
                success: function(resp){
                    $('#comboMunicipio').html(resp);
                }
            });
        }

        function buscaDadosSolicitante( solid ){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=buscaDadosSolicitante&solid="+solid,
                success: function(resp){
                    var dados = $.parseJSON(resp);
                    $('#tr_solcpf'). css("display", "");
                    $('#tr_solnome').css("display", "");
                    $('#tr_soltelefone').css("display", "");
                    $('#tr_solemail').css("display", "");
                    $('#tr_endereco').css("display", "");
                    $('#tr_solcep').css("display", "");
                    $('#tr_estado').css("display", "");
                    $('#tr_municipio').css("display", "");
                    $('#div_tabela_busca_solicitante').css("display", "");
                    $('#buscar').css("display", "none");
                    $('#ver_todos').css("display", "none");
                    $('#btnSalvar').css("display", "");
                    $('#tr_soltipopessoa').css("display", "");

                    $('#solid').val(dados.solid);
                    $('#tpsid').val(dados.tpsid);
                    $('input[name=soltipopessoa][value='+dados.soltipopessoa+']').attr('checked', true);
                    $('#solcpfcnpj').val(dados.solcpfcnpj);
                    $('#solnome').val(dados.solnome);
                    $('#soltelefone').val(dados.soltelefone);
                    $('#solemail').val(dados.solemail);
                    $('#solendereco').val(dados.solendereco);
                    $('#solcep').val(dados.solcep);
                    $('#estuf').val(dados.estuf);
                    $('#muncod').val(dados.muncod);
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
                        if(trim(resp) == 'S'){
                            alert('Operação Realizada com sucesso!');
                            window.location.reload();
                        } else {
                            alert('Não foi possível excluir o solicitante, pois o mesmo encontra-se vinculado a uma Demanda!');
                        }
                    }
                });
            }
        }

        function incluirSolicitante(solid,tarid){
            var confirma = confirm("Deseja realmente incluir o Solicitante?");
            if( confirma ){
                $.ajax({
                    type: "POST",
                    url:  'gestaodocumentos.php?modulo=principal/popupSolicitante&acao=A',
                    data: {requisicao: 'incluirSolicitante', solid: solid, tarid: $('#tarid').val()},
                    async: false,
                    success: function(resp){
                        if(trim(resp) == 'S' ){
                            alert('Operação Realizada com sucesso!');
                            window.opener.location.href='gestaodocumentos.php?modulo=principal/cadTarefa&acao=A&tarid='+$('#tarid').val();
                        } else {
                            alert('Não foi possível incluir o solicitante, pois o mesmo encontra-se vinculado a esta Demanda!');
                        }
                    }
                });
            }
        }

        function fecharPopup() {
            return window.close();
        }

        function validar_cpf_cnpj( solcpfcnpj ){
            var tamanho = $('#solcpfcnpj').val().length;
            var valido;

            if( tamanho == 14 ){
                valido = validar_cpf( solcpfcnpj );
                if(!valido){
                    alert( "CPF inválido! Favor informar um CPF válido!" );
                    $('#solcpfcnpj').focus();
                    return false;
                }else{
                    return true;
                }
            } else if( tamanho == 18 ){
                valido = validarCnpj( solcpfcnpj );
                if(!valido){
                    alert( "CNPJ inválido! Favor informar um CNPJ válido!" );
                    $('#solcpfcnpj').focus();
                    return false;
                } else {
                    return true;
                }
            }
        }

        function mascaraCPF_CNPJ( solcpfcnpj ){
            var tamanho = $('#solcpfcnpj').val().length;
            var cpf, cnpj;

            if(tamanho <= 14){
                cpf = mascaraglobal('###.###.###-##', solcpfcnpj);
                $('#solcpfcnpj').val(cpf);
            } else if(tamanho <= 18){
                cnpj = mascaraglobal('##.###.###/####-##', solcpfcnpj);
                $('#solcpfcnpj').val(cnpj);
            } else {
                alert('CPF/CNPJ Ivalido, verifique o número digitado!');
            }
        }

        function verificaTipoAcao(tipo) {
            if(trim(tipo) == 'C'){
                $('#tr_soltipopessoa').css("display", "");
                $('#tr_solcpf'). css("display", "");
                $('#tr_solnome').css("display", "");
                $('#tr_soltelefone').css("display", "");
                $('#tr_solemail').css("display", "");
                $('#tr_endereco').css("display", "");
                $('#tr_solcep').css("display", "");
                $('#tr_estado').css("display", "");
                $('#tr_municipio').css("display", "");
                $('#buscar').css("display", "none");
                $('#ver_todos').css("display", "none");
                $('#btnSalvar').css("display", "");
                $('#div_tabela_busca_solicitante').css("display", "none");

                $('#solcpfcnpj').val('');
                $('#solnome').val('');
                $('#soltelefone').val('');
                $('#solemail').val('');
                $('#solendereco').val('');
                $('#solcep').val('');
                $('#estuf').val('');
                $('#muncod').val('');
            } else {
                $('#tr_soltipopessoa').css("display", "");
                $('#tr_solcpf'). css("display", "");
                $('#tr_solnome').css("display", "");
                $('#tr_soltelefone').css("display", "none");
                $('#tr_solemail').css("display", "");
                $('#tr_endereco').css("display", "none");
                $('#tr_solcep').css("display", "none");
                $('#tr_estado').css("display", "");
                $('#tr_municipio').css("display", "");
                $('#buscar').css("display", "");
                $('#btnSalvar').css("display", "none");
                $('#btnFechar').css("display", "");
                $('#ver_todos').css("display", "");
                $('#div_tabela_busca_solicitante').css("display", "");
            }
        }

        function verificaTipoSolicitante(tipo) {
            if(tipo != ''){
                var array = tipo.split('@');
                var tpstipo = trim(array[1].replace('tpstipo_', ''));
            }

           if(trim(tpstipo) == 1){
                $('#tr_acaocadastro').css("display", "");
                $('#soltipoacaoC').attr('checked', false);
                $('#soltipoacaoP').attr('checked', false);
                $('#soltipopessoaF').attr('checked', false);
                $('#soltipopessoaJ').attr('checked', false);
           } else {
           	$('#tr_acaocadastro').css("display", "none");
                $('#tr_soltipopessoa').css("display", "none");
                $('#tr_solcpf'). css("display", "none");
                $('#tr_solnome').css("display", "none");
                $('#tr_soltelefone').css("display", "none");
                $('#tr_solemail').css("display", "none");
                $('#tr_endereco').css("display", "none");
                $('#tr_solcep').css("display", "none");
                $('#tr_estado').css("display", "none");
                $('#tr_municipio').css("display", "none");
                $('#buscar').css("display", "none");
                $('#ver_todos').css("display", "none");
                $('#div_tabela_busca_solicitante').css("display", "none");
            }
            
            if(tpstipo == 2){
                $('#tr_buscar_instituicao').css("display", "");
            } else {
                $('#tr_buscar_instituicao').css("display", "none");
            }

            if(tpstipo == 3){
                $('#tr_buscar_entidade').css("display", "");
            } else {
                $('#tr_buscar_entidade').css("display", "none");
            }


            if(tpstipo == 4){
                $('#tr_buscar_area_mec').css("display", "");
            } else {
                $('#tr_buscar_area_mec').css("display", "none");
            }
            
            if(tpstipo == 5){
                $('#tr_buscar_mantenedora').css("display", "");
            } else {
                $('#tr_buscar_mantenedora').css("display", "none");
            }
        }

        function verificaTipoPessoa(tipo){
            if($("input[name='soltipoacao']:checked").val() == 'C' && (tipo == 'J' || tipo == 'F')){
                $('#tr_solcpf'). css("display", "");
                $('#tr_solnome').css("display", "");
                $('#tr_soltelefone').css("display", "");
                $('#tr_solemail').css("display", "");
                $('#tr_endereco').css("display", "");
                $('#tr_solcep').css("display", "");
                $('#tr_estado').css("display", "");
                $('#tr_municipio').css("display", "");
                $('#div_tabela_busca_solicitante').css("display", "none");
                $('#buscar').css("display", "none");
                $('#ver_todos').css("display", "none");
            }
            jQuery.ajax({
                type: 'POST',
                url: 'gestaodocumentos.php?modulo=principal/popupSolicitante&acao=A',
                data: jQuery('#formulario').serialize(),
                async: false,
                success: function(data){
                        jQuery("#div_tabela_busca_solicitante").html(data);
                        divCarregado();
                }
            });
            if(tipo == 'J'){
                $('#td_solcpfcnpj').html('CNPJ:');
                $('#td_solnome').html('Razão Social:');
            } else {
                $('#td_solcpfcnpj').html('CPF:');
                $('#td_solnome').html('Nome do Solicitante:');
            }
        }

        function enviaForm(){
            var tpsid = $('#tpsid').val();
            var array = tpsid.split('@');
            var tpstipo = trim(array[1].replace('tpstipo_', ''));
            
            if(trim(tpstipo) == 1){
                if(trim($('input:radio[name=soltipoacao]:checked').val()) == 'undefined'){
                    alert('O campo "Tipo de Ação" é um campo obrigatório!');
                    $('input:radio[name=soltipoacao]').focus();
                    return false;
                }

                if(trim($('input:radio[name=soltipopessoa]:checked').val()) == 'undefined'){
                    alert('O campo "Tipo de Pessoa" é um campo obrigatório!');
                    $('input:radio[name=soltipopessoa]').focus();
                    return false;
                }

                if($('#solnome').val() == ''){
                    if($('input:radio[name=soltipopessoa]:checked').val() == 'F'){
                            alert('O campo "Nome" é um campo obrigatório!');
                    } else {
                            alert('O campo "Razão Social" é um campo obrigatório!');
                    }
                    $('#solnome').focus();
                    return false;
                }
            }

            if(trim(tpstipo) == 2){
                if(trim($('#iesid').val()) == 'undefined'){
                    alert('O campo "Instituição" é um campo obrigatório!');
                    $('#ogsid').focus();
                    return false;
                }
            }
            
            if(trim(tpstipo) == 3){
                if(trim($('#ogsid').val()) == 'undefined'){
                    alert('O campo "Órgão" é um campo obrigatório!');
                    $('#ogsid').focus();
                    return false;
                }
            }

            if(trim(tpsid) == 4){
                if($('#uamid').val() == ''){
                    alert('O campo "Área MEC" é um campo obrigatório!');
                    $('#uamid').focus();
                    return false;
                }
            }
            $('#requisicao').val('salvarDadosSolicitante');
            $('#formulario').submit();
        }

        function pesquisarSolicitante(param){
        	$('#requisicao').val('pesquisarSolicitante');
            if(trim(param) == 'filtro'){
                $('#tipo_filtro').val('filtro');
            } else {
                $('#tipo_filtro').val('todos');
            }
            divCarregando();
            jQuery.ajax({
                type: 'POST',
                url: 'gestaodocumentos.php?modulo=principal/popupSolicitante&acao=A',
                data: jQuery('#formulario').serialize(),
                async: false,
                success: function(data){
                        jQuery("#div_tabela_busca_solicitante").html(data);
                        divCarregado();
                }
            });
        }
    </script>
    </head>
    <br>
    <form name="formulario" id="formulario" action="" method="POST">
       	<input type="hidden" name="requisicao" id="requisicao" value="" />
        <input type="hidden" name="solid" id="solid" value="" />
        <input type="hidden" name="tipo_filtro" id="tipo_filtro" value="" />
        <input type="hidden" name="tarid" id="tarid" value="<?PHP echo $_GET['tarid']; ?>" />
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <tr>
                <td class="subtitulodireita" colspan="2"><center> <h3>Cadastro de Solicitante</h3></center></td>
            </tr>
            <tr>
                <td class="subtitulodireita" width="30%">Tipo de Solicitante:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  'tpsid_'||tpsid||'@tpstipo_'||tpstipo as codigo,
                                    tpsdsc as descricao
                            FROM gestaodocumentos.tiposolicitante
                            ORDER BY tpstipo, tpsid
                        ";
                        $db->monta_combo("tpsid", $sql, 'S', 'Selecione...', 'verificaTipoSolicitante', '', '', '420', 'S', 'tpsid', false, null, 'Tipo de Solicitante');
                    ?>
                </td>
            </tr>
            <tr id="tr_acaocadastro" style="display:none;">
                <td class="subtitulodireita">Tipo de Ação:</td>
                <td>
                    <input type="radio" id="soltipoacaoC" title="Tipo de Ação" name="soltipoacao" value="C" onclick="verificaTipoAcao('C');">Novo Cadastro
                    &nbsp;&nbsp;
                    <input type="radio" id="soltipoacaoP" title="Tipo de Ação" name="soltipoacao" value="P" onclick="verificaTipoAcao('P');">Pesquisar
                    &nbsp;&nbsp;
                    <img src="/imagens/obrig.gif">
                </td>
            </tr>
            <?PHP ?>
            <tr id="tr_soltipopessoa" style="display:none;">
                <td class="subtitulodireita">Tipo de Pessoa:</td>
                <td>
                    <input type="radio" id="soltipopessoaJ" title="Tipo de Pessoa" name="soltipopessoa" value="J" onclick="verificaTipoPessoa('J');">
                    <label for="soltipopessoaJ"> Jurídica</label>
                    &nbsp;&nbsp;
                    <input type="radio" id="soltipopessoaF" title="Tipo de Pessoa" name="soltipopessoa" value="F" onclick="verificaTipoPessoa('F');">
                    <label for="soltipopessoaF"> Física</label>
                    &nbsp;&nbsp;
                    <img src="/imagens/obrig.gif">
                </td>
            </tr>
            <tr id="tr_solcpf" style="display:none;">
                <td class = "subtitulodireita"id="td_solcpfcnpj">CPF:</td>
                <td><?PHP echo campo_texto('solcpfcnpj', 'N', 'S', 'CPF', '56', '18', '', '', '', '', '', 'id="solcpfcnpj"', 'mascaraCPF_CNPJ(this.value)', $solcpfcnpj, 'validar_cpf_cnpj(this.value);'); ?></td>
            </tr>
            <tr id="tr_solnome" style="display:none;">
                <td class = "subtitulodireita" id="td_solnome">Nome do Solicitante</td>
                <td><?PHP echo campo_texto('solnome', 'S', 'S', '', '56', '100', '', '', '', '', '', 'id="solnome"', '', $solnome); ?>
                </td>
            </tr>
            <tr id="tr_soltelefone" style="display:none;">
                <td class = "subtitulodireita">Telefone de Contato do Solicitante:</td>
                <td><?PHP echo campo_texto('soltelefone', 'N', 'S', 'Telefone de Contato do Solicitante', '56', '30', '##-####-####', '', '', '', '', 'id="soltelefone"', '', $soltelefone); ?>
                </td>
            </tr>
            <tr id="tr_solemail" style="display:none;">
                <td class = "subtitulodireita">Email de Contato do Solicitante:</td>
                <td><?PHP echo campo_texto('solemail', 'N', 'S', 'E-mail de Contato do Solicitante', '56', '100', '', '', '', '', '', 'id="solemail"', '', $solemail); ?>
                </td>
            </tr>
            <tr id="tr_endereco" style="display:none;">
                <td class = "subtitulodireita">Endereço do Solicitante:</td>
                <td><?PHP echo campo_texto('solendereco', 'N', 'S', 'Endereço do Solicitante', '56', '150', '', '', '', '', '', 'id="solendereco"', '', $solendereco); ?></td>
            </tr>
            <tr id="tr_solcep" style="display:none;">
                <td class = "subtitulodireita">CEP:</td>
                <td><?PHP echo campo_texto('solcep', 'N', 'S', 'CEP', '56', '8', '########', '', '', '', '', 'id="solcep"', '', $solcep); ?></td>
            </tr>
            <tr id="tr_estado" style="display:none;">
                <td class = "subtitulodireita">Estado:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  e.estuf as codigo,
                                    e.estdescricao as descricao
                            FROM territorios.estado e
                            ORDER BY e.estdescricao asc
                        ";
                        $db->monta_combo("estuf", $sql, 'S', 'Selecione...', 'atualizaComboMun', '', '', '420', 'N', 'estuf', false, null, 'Estado');
                    ?>
                </td>
            </tr>
            <tr id="tr_municipio" style="display:none;">
                <td class = "subtitulodireita">Município:</td>
                <td id="comboMunicipio">
                    <?PHP
                        $sql = "
                            SELECT  muncod as codigo,
                                    mundescricao as descricao
                            FROM territorios.municipio
                            ORDER BY mundescricao asc
                        ";
                        $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', '420', 'N', 'muncod', false, null, 'Município');
                    ?>
                </td>
            </tr>
            <tr id="tr_buscar_area_mec" style="display: none;">
                <td class = "subtitulodireita">Área MEC:</td>
                <td  id="area_mec">
                    <?PHP
                        $sql = "
                            SELECT  uamid as codigo,
                                    uamsigla || ' - ' || uamdsc as descricao
                            FROM public.unidadeareamec
                            WHERE uamid NOT IN (2,5,10,11,12,13,16)
                            ORDER BY descricao asc
                        ";
                        $db->monta_combo("uamid", $sql, 'S', 'Selecione...', '', '', '', '420', 'S', 'uamid', false, $uamid, 'Unidade MEC');
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
            <tr id="tr_buscar_entidade" style="display: none;">
                <td class = "subtitulodireita">Buscar Órgão:</td>
                <td>
                    <input type="button" name="buscar_orgao" id="buscar_orgao" onclick="abrirPopupOrgao('radio');" value="Buscar Órgão">
                    <div id="div_orgao" >
                        <?PHP
                            $resp = "<span style=\"color: #1874CD;\"\">".$rs[0]['ogsid']." - ".$rs[0]['tpodsc']." - ".$rs[0]['ogsdsc']."</span>";
                            $resp .= "<input type=\"hidden\" name=\"ogsid\" id=\"ogsid\" value=\"{$rs[0]['ogsid']}\" />";
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
                    <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisarSolicitante('filtro');" style="display: none;">
                    <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisarSolicitante('todos');" style="display: none;">
                    <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="enviaForm();">
                    <input type="button" name="btnFechar" value="Cancelar" id="btnFechar" onclick="fecharPopup();">
                </td>
            </tr>
        </table>
    </form>
    <br>
    <div id="div_tabela_busca_solicitante" style="display: none;">
        <?PHP
            $sql = "SELECT  	'<img align=\"absmiddle\" src=\"/imagens/gif_inclui.gif\" style=\"cursor: pointer\" onclick=\"incluirSolicitante('||s.solid||');\" title=\"Incluir Solicitante\">
            					<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscaDadosSolicitante('||s.solid||');\" title=\"Editar Solicitante\">
                    			<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirSolicitante('||s.solid||');\" title=\"Excluir Solicitante\" >' AS acao,
	                        	s.solnome,
		                        CASE WHEN s.solcpfcnpj <> ''
		                            THEN
		                                CASE WHEN CHAR_LENGTH(s.solcpfcnpj) = 11
		                                    THEN trim( replace( to_char( cast(s.solcpfcnpj as bigint), '000:000:000-00' ), ':', '.' ) )
		                                    ELSE trim( replace( to_char( cast(s.solcpfcnpj as bigint), '00:000:000/0000-00' ), ':', '.' ) )
		                                END
		                            ELSE ''
		                        END AS solcpfcnpj,
		                        s.soltelefone,
		                        s.solemail
                	FROM 		gestaodocumentos.solicitantepessoa s
	                ORDER BY 	s.solid";

            $cabecalho = array("Ação", "Descrição", "CPF/CNPJ", "Telefone", "E-mail");
            $alinhamento = Array('center', 'left', 'left', 'left', 'left');
            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', '', $alinhamento);
        ?>
    </div>

<?PHP if($_REQUEST['solid']){ ?>
	<script type="text/javascript">buscaDadosSolicitante(<?PHP echo $_REQUEST['solid']; ?>);</script>
<?PHP } ?>