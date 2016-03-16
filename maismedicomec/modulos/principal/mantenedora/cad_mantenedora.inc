<?PHP
    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Mais Médicos - MEC', 'Cadastro da Mantenedora' );

    $abacod_tela    = ABA_CADASTRO_MANTENEDORA;
    $url            = 'maismedicomec.php?modulo=principal/mantenedora/cad_mantenedora&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

    if($_SESSION['maismedicomec']['mntid'] == ''){
        $mntid = $_REQUEST['mntid'];

        if( $_SESSION['maismedicomec']['mntid'] == '' ){
            $_SESSION['maismedicomec']['mntid'] = $mntid;
        }
    }else{
        $mntid = $_SESSION['maismedicomec']['mntid'];
    }

    if( $mntid != '' ){
        $dados = buscarDadosMantenedora( $mntid );
        if( $dados['mntid'] != '' ){ 
            $habilita_mntid = 'N';
        }else{
            $habilita_mntid = 'S';
        }
        
        if($dados['rplsexo'] == 'M'){
            $checked_m = 'checked="checked"';
        }elseif($dados['rplsexo'] == 'F'){
            $checked_f = 'checked="checked"';
        }else{
            $checked_m = '';
            $checked_f = '';
        }
    }
    
    $perfil = pegaPerfilGeral();
?>

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
    
    //alert("ATENÇÃO \n\n1- Indicar Mantida para autorização de curso de medicina\n ou \n 2- Credenciar Nova Mantida para autorização de curso de medicina. \nNo caso 1, preencher a Aba Mantida Indicada.\n No caso 2, preencher a Aba Credenciar nova mantida.");
    function abrirPopupSelecaoMantenedora(){
        var url = 'maismedicomec.php?modulo=principal/mantenedora/popup_selecione_mantenedora&acao=A';        
        window.open(url, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=650');
    }

    function atualizaComboMunicipioMantenedora( estuf ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboMunicipioMantenedora&estuf="+estuf,
            async: false,
            success: function(resp){
                $('#td_combo_municipio').html(resp);
            }
        });
    }

    function buscarDadosRepresentanteLegal( cpf ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarDadosRepresentanteLegal&rplcpf="+cpf,
            success: function(resp){
                var dados = $.parseJSON(resp);
                //$LIMPA FORMULARIO. (APENAS CAMPOS REFERENTES AO REPRESENTANTE LEGAL)
                $('#rpldsc').val('');
                $('#rplrg').val('');
                $('input[name="rplsexo"]').attr('checked', false);
                $('#rplorgaoexprg').val('');
                $('#rplestuf').val('');
                $('#rpltelcomercial').val('');
                $('#rpltelcelular').val('');
                $('#rplemail').val('');

                $.each(dados, function(i, v){
                    $('#'+i).val(v);

                    $('#rplsexo_m').val('M');
                    $('#rplsexo_f').val('F');
                    if( i == 'rplsexo' && v == 'M' ){
                        $('#rplsexo_m').attr('checked', true);
                    }else{
                        $('#rplsexo_f').attr('checked', true);
                    }
                });
            }
        });
    }
    
    function buscarEndereceCEP( cep ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarEndereceCEP&cep="+cep,
            success: function(resp){
                var dados = $.parseJSON(resp);
                $('#mntlogradouro').val(dados.logradouro);
                $('#mntbairro').val(dados.bairro);
                $('#estuf').val(dados.estado);
                $('#muncod').val(dados.muncod);
            }
        });
    }

    function resetFromulario(){
        $('#selecionar').attr('disabled', false);
        $.each($("input[type=text], select, textarea, input[type=radio], input[type=checkbox]"), function(i,v){
            if( $(this).attr('type') != 'radio' && $(this).attr('type') != 'checkbox'){
                $(v).val('');
            }
            if( $(this).attr('type') == 'radio' ){
                $(v).attr('checked', false);
            }
            if( $(this).attr('type') == 'checkbox' ){
                $(v).attr('checked', false);
            }
        });
    }
    
    function resetFromulario2(){
    	location.href="maismedicomec.php?modulo=principal/mantenedora/cad_mantenedora&acao=A";
    }

    function salvarDadosMantenedora(){
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
            $('#requisicao').val('salvarDadosMantenedora');
            $('#formulario').submit();
        }
    }

    function verificarCNPJValido( cnpj ){
        var valido = validarCnpj( cnpj );

        if(!valido){
            alert( "CNPJ inválido! Favor informar um CNPJ válido!" );
            $('#mntcnpj').focus();
            return false;
        }
    }
    
    function verificarCPFValido( cpf ){
        var valido = validar_cpf( cpf );
        
        if(!valido){
            alert( "CPF inválido! Favor informar um CPF válido!" );
            $('#rplcpf').focus();
            return false;
        }
    }

    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/lista_grid_mantenedora&acao=A';
        janela.focus();
    }

</script>

<?PHP
    $perfil = pegaPerfilGeral($_SESSION['usucpf']);

    if( in_array(PERFIL_RCS_PROFESSOR, $perfil) ){
        $cpf = $_SESSION['usucpf'];
    }else{
        $cpf = $dados['srpnumcpf'];
    }
    if( $cpf != '' ){
       buscaUnidadesAssociadas( $cpf );
    }
?>


<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="rplid" name="rplid" value="<?=$dados['rplid'];?>"/>
    <input type="hidden" id="mm_mntid" name="mm_mntid" value="<?=$dados['mm_mntid'];?>"/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Identificação </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Mantenedora: </td>
            <td colspan="1" width="17%">
                <?PHP
                    $mntid = $dados['mntid'];
                    echo campo_texto('mntid', 'S', $habilita_mntid, 'Código Mantenedora', 29, 10, '', '', '', '', 0, 'id="mntid"', '', $mntid, '', null);
                ?>
            </td>
            <?PHP
                if( $habilita_mntid == '' || $habilita_mntid == 'S' ){ 
            ?>
                <td>
                    <input type="button" id="selecionar" name="selecionar" value="Selecionar Mantenedora" onclick="abrirPopupSelecaoMantenedora();">
                </td>
            <?PHP
                }else{
            ?>
                <td>
                    <input type="button" id="selecionar" name="selecionar" value="Selecionar Mantenedora" disabled="disables">
                </td>
            <?PHP
                }
            ?>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> CPNJ: </td>
            <td colspan="2">
                <?PHP
                    $mntcnpj = $dados['mntcnpj'];
                    echo campo_texto('mntcnpj', 'S', 'N', 'CNPJ', 29, 18, '##.###.###/####-##', '', '', '', 0, 'id="mntcnpj"', '', $srpnumcpf, 'verificarCNPJValido(this.value);', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Razão Social: </td>
            <td colspan="2">
                <?PHP
                    $mntrazaosocial = $dados['mntrazaosocial'];
                    echo campo_texto('mntrazaosocial', 'S', 'N', 'Razão Social', 89, 100, '', '', '', '', 0, 'id="mntrazaosocial"', '', $mntrazaosocial, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Sigla: </td>
            <td colspan="2">
                <?PHP
                    $mntsigla = $dados['mntsigla'];
                    echo campo_texto('mntsigla', 'N', 'S', 'Sigla', 29, 100, '', '', '', '', 0, 'id="mntsigla"', '', $mntsigla, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Endereço </td>
        </tr>

        <tr>
            <td class ="SubTituloDireita"> CEP: </td>
            <td colspan="2">
               <?PHP
                    $mntcep = $dados['mntcep'];
                    echo campo_texto('mntcep', 'S', 'S', 'CEP', 29, 9, '#####-###', '', 'left', '', 0, 'id="mntcep"', '', $mntcep, 'buscarEndereceCEP(this.value)', '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Logradouro: </td>
            <td colspan="2">
               <?PHP
                    $mntlogradouro = $dados['mntlogradouro'];
                    echo campo_texto('mntlogradouro', 'S', 'S', 'Logradouro', 89, 100, '', '', 'left', '', 0, 'id="mntlogradouro"', '', $mntlogradouro, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Complemento: </td>
            <td colspan="2">
               <?PHP
                    $mntcomplementoend = $dados['mntcomplementoend'];
                    echo campo_texto('mntcomplementoend', 'N', 'S', 'Complemento', 89, 100, '', '', 'left', '', 0, 'id="mntcomplementoend"', '', $mntcomplementoend, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Bairro: </td>
            <td colspan="2">
               <?PHP
                    $mntbairro = $dados['mntbairro'];
                    echo campo_texto('mntbairro', 'S', 'S', 'Bairro', 89, 100, '', '', 'left', '', 0, 'id="mntbairro"', '', $mntbairro, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> UF: </td>
            <td colspan="2">
                <?PHP
                    $estuf = $dados['mntestuf'];
                    $sql = "
                         SELECT  estuf as codigo,
                                 estuf as descricao
                         FROM territorios.estado
                         ORDER BY estuf
                     ";
                     $db->monta_combo('estuf', $sql, 'S', "Selecione...", 'atualizaComboMunicipioMantenedora', '', '', 300, 'S', 'estuf', '', $estuf, 'UF');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Município: </td>
            <td id="td_combo_municipio" colspan="2">
                <?PHP
                    $muncod = $dados['mntmuncod'];
                    $sql = "
                        SELECT  muncod AS codigo,
                                mundescricao AS descricao
                        FROM territorios.municipio
                        ORDER BY descricao
                    ";
                    $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', 300, 'S', 'muncod', '', $muncod, 'Município');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone 1: </td>
            <td colspan="2">
               <?PHP
                    $textoDica = 'O telefone é composto por código de área (DDD) - Prefixo - Número do telefone. Ex: 61-2200-1111.';

                    $mnttelefone01 = $dados['mnttelefone01'];
                    echo campo_texto('mnttelefone01', 'N', 'S', 'Telefone 1', 29, 12, '##-####-####', '', 'left', $textoDica, 0, 'id="mnttelefone01"', '', $mnttelefone01, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone 2: </td>
            <td colspan="2">
               <?PHP
                    $mnttelefone02 = $dados['mnttelefone02'];
                    echo campo_texto('mnttelefone02', 'N', 'S', 'Telefone 2', 29, 12, '##-####-####', '', 'left', $textoDica, 0, 'id="mnttelefone02"', '', $mnttelefone02, null, '', null);
                ?>

            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone 3: </td>
            <td colspan="2">
               <?PHP
                    $mnttelefone03 = $dados['mnttelefone03'];
                    echo campo_texto('mnttelefone03', 'N', 'S', 'Telefone 3', 29, 12, '##-####-####', '', 'left', $textoDica, 0, 'id="mnttelefone03"', '', $mnttelefone03, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> E-mail Institucional: </td>
            <td colspan="2">
                <?PHP
                    $mntemail = $dados['mntemail'];
                    echo campo_texto('mntemail', 'N', 'S', 'E-mail', 89, 100, '', '', '', '', 0, 'id="mntemail"', '', $mntemail, '', null);
                ?>
            </td>
        </tr>

        <!-- DIRIGENTE LEGAL -->
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Dirigente Legal </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> CPF: </td>
            <td colspan="2">
                <?PHP
                    $rplcpf = $dados['rplcpf'];
                    //echo campo_texto('rplcpf', 'S', 'S', 'CPF', 29, 18, '###.###.###-##', '', '', '', 0, 'id="rplcpf"', '', $rplcpf, 'buscarDadosRepresentanteLegal(this.value); verificarCPFValido(this.value);', null);
                    echo campo_texto('rplcpf', 'S', 'S', 'CPF', 29, 18, '###.###.###-##', '', '', '', 0, 'id="rplcpf"', '', $rplcpf, "pegaNome(this)", null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Nome: </td>
            <td colspan="2">
                <?PHP
                    $rpldsc = $dados['rpldsc'];
                    echo campo_texto('rpldsc', 'S', 'S', 'Nome', 89, 100, '', '', '', '', 0, 'id="rpldsc"', '', $rpldsc, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Sexo: </td>
            <td colspan="2">
                <input type="radio" name="rplsexo" id="rplsexo_m" value="M" class="obrigatorio" <?=$checked_m;?>> Masculino
                <input type="radio" name="rplsexo" id="rplsexo_f" value="F" class="obrigatorio" <?=$checked_f;?>> Feminino
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> RG: </td>
            <td colspan="2">
               <?PHP
                    $rplrg = $dados['rplrg'];
                    echo campo_texto('rplrg', 'S', 'S', 'RG', 29, 20, '####################', '', 'left', '', 0, 'id="rplrg"', '', $rplrg, '', '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Órgão Expeditor: </td>
            <td colspan="2">
               <?PHP
                    $rplorgaoexprg = $dados['rplorgaoexprg'];
                    echo campo_texto('rplorgaoexprg', 'S', 'S', 'Órgão Expeditor', 29, 10, '', '', 'left', '', 0, 'id="rplorgaoexprg"', '', $rplorgaoexprg, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> UF: </td>
            <td colspan="2">
                <?PHP
                    $rplestuf = $dados['rplestuf'];
                    $sql = "
                         SELECT  estuf as codigo,
                                 estuf as descricao
                         FROM territorios.estado
                         ORDER BY estuf
                     ";
                     $db->monta_combo('rplestuf', $sql, 'S', "Selecione...", '', '', '', 210, 'S', 'rplestuf', '', $rplestuf, 'Dirigente Legal - UF');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone Comercial: </td>
            <td colspan="2">
               <?PHP
                    $textoDica = 'O telefone é composto por código de área (DDD) - Prefixo - Número do telefone. Ex: 61-2200-1111.';

                    $rpltelcomercial = $dados['rpltelcomercial'];
                    echo campo_texto('rpltelcomercial', 'S', 'S', 'Telefone Comercial', 29, 12, '##-####-####', '', 'left', '', 0, 'id="rpltelcomercial"', '', $rpltelcomercial, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone Celular: </td>
            <td colspan="2">
               <?PHP
                    $rpltelcelular = $dados['rpltelcelular'];
                    echo campo_texto('rpltelcelular', 'S', 'S', 'Telefone Celular', 29, 12, '##-####-####', '', 'left', '', 0, 'id="rpltelcelular"', '', $rpltelcelular, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> E-mail Institucional: </td>
            <td colspan="2">
                <?PHP
                    $rplemail = $dados['rplemail'];
                    echo campo_texto('rplemail', 'S', 'S', 'E-mail', 89, 100, '', '', '', '', 0, 'id="rplemail"', '', $rplemail, '', null);
                ?>
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
                
                <?PHP
                	$docid = criaDocidMantenedora( $_SESSION['maismedicomec']['mntid'] );
                	$esdid = pegaEstadoAtualWorkflow( $docid );
                	
                    if( in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil) || in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) ){
                ?>
	                <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosMantenedora();"/>
	                <input type="button" id="cancela" name="cancela" value="Cancelar" onclick="resetFromulario2();"/>
                <?PHP
                    }elseif( in_array(PERFIL_IES, $perfil) && (!$esdid || $esdid == WF_EM_PREENCHIMENTO_IES || $esdid == WF_EM_AJUSTE_IES) ){
                    	if(date('Ymd') < 20150124){
	                    	?>
	                    	<input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosMantenedora();"/>
		                	<input type="button" id="cancela" name="cancela" value="Cancelar" onclick="resetFromulario2();"/>	
	                    	<?
						}
                    	
                    }
                ?>
                
                <input type="button" id="voltar" name="voltar" value="voltar" onclick="voltarPaginaPrincipal();"/>
                
                <?PHP
                    if( in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil) || in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) ){
                ?>
                        
                <?PHP
                    }
                ?>
            </td>
        </tr>
    </table>
</form>


<script>
 
		function pegaNome(obj) {
                var cpf = obj.value;
                var nome = document.getElementById('rpldsc');
                //var email = document.getElementById('monemailicp');
                //var telefone = document.getElementById('montelefoneicp');

                //valida cpf
                if (!validar_cpf(cpf)) {
                    alert('CPF inválido!');
                    obj.value = "";
                    nome.value = "";
                    //obj.focus();
                    return false;
                }
                
                $.ajax({
	                type    : "POST",
	                url     : '/includes/webservice/cpf.php',
	                data    : 'ajaxCPF=' + cpf,
	                success: function(resp){
	                    //alert(resp);
                        if (resp) {
                            linhaItem = resp.split('|');
                            colunaItem = linhaItem[0].split('#');

                            if (colunaItem[1]) {
                                nome.value = colunaItem[1];
                            } else {
                                alert("CPF não existe!");
                                obj.value = "";
                                nome.value = "";
                                //obj.focus();
                            }

                        } else {
                            alert("CPF não existe!");
                            obj.value = "";
                            nome.value = "";
                            //obj.focus();
                        }
	                }
	            });

                
            }
 </script>