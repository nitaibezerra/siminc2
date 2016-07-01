<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    if($_SESSION['maismedicomec']['mntid'] == ''){
        $db->sucesso("principal/mantenedora/cad_mantenedora", '', "Ocorreu um problema, é necessário o preenchimento dos dados da Mantida. Tente novamente!");
    }
    
    verificaMantenedora();
    
    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Mais Médicos - MEC', 'Cadastro da Mantenedora' );

    $abacod_tela    = ABA_CADASTRO_MANTENEDORA;
    $url            = 'maismedicomec.php?modulo=principal/mantenedora/cad_corpo_dirigente&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

    if($cpdid == ''){
        $cpdid = $_REQUEST['cpdid'];
    }

    if( $cpdid != '' ){
        $dados = buscarDadosCorpoDirigente( $cpdid );

        if( $dados['dmtid'] != '' ){
            $checked_mnt = 'checked="checked"';
            $style = " style=\"display: none;\" ";
        }else
            if( $dados['dmdid'] != '' ){
            $checked_mtd = 'checked="checked"';
            $style = "";
        }
    }else{
        $checked_mnt = '';
        $checked_mtd = '';
        $style = " style=\"display: none;\" ";
    }
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

    function atualizarComboMantida( param ){
        if( param == 'D' ){
            $('#tr_combo_mantida').css('display', '');
            $("#mtdid").attr('class', 'obrigatorio');
        }else{
            $('#tr_combo_mantida').css('display', 'none');
            $("#mtdid").attr('class', '');
        }
    }

    function buscarDadosDirigente( cpf ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarDadosDirigente&cpdcpf="+cpf,
            success: function(resp){
                var dados = $.parseJSON(resp);
                if(dados != 'V'){
                    $('#cpdnome').val(dados.cpdnome);
                    $('#cpdcargo').val(dados.cpdcargo);
                    $('#cpdtelefonecomercial').val(dados.cpdtelefonecomercial);
                    $('#cpdtelcelular').val(dados.cpdtelcelular);
                    $('#cpdemail').val(dados.cpdemail);
                }else{
                    $('#cpdnome').val('');
                    $('#cpdcargo').val('');
                    $('#cpdtelefonecomercial').val('');
                    $('#cpdtelcelular').val('');
                    $('#cpdemail').val('');
                }
            }
        });
    }

    function buscarDadosEditarCorpoDirigente( cpdid ){
        resetFromulario();
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarDadosEditarCorpoDirigente&cpdid="+cpdid,
            success: function(resp){
                var dados = $.parseJSON(resp);

                $.each(dados, function(i, v){
                    $('#'+i).val(v);

                    $('#mnt').val('M');
                    $('#mtd').val('D');
                    if( i == 'mntid' && v != '' ){
                        $('#mnt').attr('checked', true);
                        $('#tr_combo_mantida').css('display', 'none');
                        $("#mtdid").attr('class', '');
                    }else
                        if(  i == 'mtdid' && v != ''  ){
                            $('#mtd').attr('checked', true);
                            $('#tr_combo_mantida').css('display', '');
                            $("#mtdid").attr('class', 'obrigatorio');
                    }
                });
            }
        });
    }

	
    function excluirDadosMantidaCorpoDirigente( cpdid ){
        //buscarDadosEditarCorpoDirigente( cpdid );

        var confirma = confirm("Deseja realmente excluir o Registro?");

        if( confirma ){
            //var dmdid = $('#dmdid').val();
            //var dmtid = $('#dmtid').val();

            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=excluirDadosMantidaCorpoDirigente&cpdid="+cpdid+'&dmdid='+dmdid+'&dmtid='+dmtid,
                success: function(resp){
                    if( trim( resp ) == 'OK' ){
                        alert('Operação Realizada com sucesso!');
                        location.href = 'maismedicomec.php?modulo=principal/mantenedora/cad_corpo_dirigente&acao=A';
                        //location.reload (true);
                    }
                }
            });
        }
    }
	
	/*
	function excluirDadosMantidaCorpoDirigente( cpdid ){
        var confirma = confirm("Deseja realmente excluir o Registro?");
        if( confirma ){
        	window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/cad_corpo_dirigente&acao=A&requisicao=excluirDadosMantidaCorpoDirigente&cpdid='+cpdid;
        }
    }
	*/
	
    function resetFromulario(){
        $.each($("input[type=text], input[type=hidden], select, textarea, input[type=radio], input[type=checkbox]"), function(i,v){
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
    	location.href="maismedicomec.php?modulo=principal/mantenedora/cad_corpo_dirigente&acao=A";
	}
	
    function salvarDadosCorpoDirigente(){
        var erro;
        var radio_entro;
        var check_entro;
        var campos = '';

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

                    if(!radio_box.val() && (name != radio_entro) ){
                        erro = 1;
                        radio_entro = name;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
            }else
                if( $(this).attr('type') == 'checkbox' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var check_box = $('input:checkbox[name='+name+']:checked');

                    if(!check_box.val() && (name != check_entro) ){
                        erro = 1;
                        check_entro = name;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
            }
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }


        if(!erro){
            $('#requisicao').val('salvarDadosCorpoDirigente');
            $('#formulario').submit();
        }
    }

    function verificarCPFValido( cpf ){
        var valido = validar_cpf( cpf );
        if(!valido){
            alert( "CPF inválido! Favor informar um CPF válido!" );
            $('#cpdcpf').focus();
            return false;
        }
    }

    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'maismedicomec.php?modulo=principal/inicio_direcionamento&acao=A';
        janela.focus();
    }
    atualizarComboMantida('D');
</script>

<?php $mntid = $_SESSION['maismedicomec']['mntid'];
$sql = "select * from maismedicomec.mantenedora where mntid = ".$mntid;
$dados_mantenedora = $db->carregar($sql);

?>
   <table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">

        <tr>
            <td class ="SubTituloDireita" width="35%">Código mantenedora: </td>
            <td><?php echo $mntid ?></td>
        </tr>
        
          <tr>
            <td class ="SubTituloDireita" width="35%">Razão social</td>
            <td><?php echo $dados_mantenedora[0][mntrazaosocial];?></td>
        </tr>
   </table>


<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>

    <input type="hidden" id="dmtid" name="dmtid" value="<?=$dados['dmtid'];?>"/>
    <input type="hidden" id="dmdid" name="dmdid" value="<?=$dados['dmdid'];?>"/>
    
                <input type="hidden" name="mntid_mtdid" id="mtd" value="D" class="obrigatorio" title="Selecione - Mantida" onclick="atualizarComboMantida('D');">

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
<!--        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Pertence ao Corpo Dirigente </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> Faz parte de qual Corpo Dirigente: </td>
            <td colspan="2">
                
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>-->
        <tr id="tr_combo_mantida">
            <td class ="SubTituloDireita"> Mantida Indicada/Candidata: </td>
            <td colspan="2">
                <?PHP
                    $mtdid = $dados['mtdid'];
                    $sql = "
                                    SELECT  m.mtdid AS codigo,
                                            m.mtddsc || ' - ' || mundescricao AS descricao
                                    FROM maismedicomec.mantida AS m
                                    JOIN maismedicomec.mantidamunicipio AS mt ON mt.mtdid = m.mtdid
                                    JOIN territorios.municipio AS mu ON mu.muncod = mt.muncod
                                    WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
                                    group by m.mtdid, m.mtddsc, mundescricao

                                    union

                                    SELECT  m.mtdid AS codigo,
                                            m.mtddsc || ' - ' || mundescricao AS descricao
                                    FROM maismedicomec.mantidacandidata AS m
                                    JOIN maismedicomec.mantidamunicipiocandidata AS mt ON mt.mtdid = m.mtdid
                                    JOIN territorios.municipio AS mu ON mu.muncod = mt.muncod
                                    WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
                                    group by m.mtdid, m.mtddsc, mundescricao

                                    ORDER BY 2
                                ";
                    $db->monta_combo('mtdid', $sql, 'S', "Selecione...", '', '', '', 390, 'S', 'mtdid', '', $mtdid, 'Mantida');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Identificação </td>
        </tr>
<!--        <tr>
            <td class="SubTituloDireita" width="30%"> Código Dirigente: </td>
            <td colspan="2">
                <?PHP
                    $cpdid = $dados['cpdid'];
                    echo campo_texto('cpdid', 'N', 'N', 'Código Dirigente', 29, 29, '', '', '', '', 0, 'id="cpdid"', '', $cpdid, '', null);
                ?>
            </td>
        </tr>-->
        <tr>
            <td class="SubTituloDireita" width="30%"> CPF: </td>
            <td colspan="2">
                <?PHP
                    $cpdcpf = $dados['cpdcpf'];
                    //echo campo_texto('cpdcpf', 'S', 'S', 'CPF', 29, 18, '###.###.###-##', '', '', '', 0, 'id="cpdcpf"', '', $cpdcpf, 'verificarCPFValido(this.value); buscarDadosDirigente(this.value);', null);
                    echo campo_texto('cpdcpf', 'S', 'S', 'CPF', 29, 18, '###.###.###-##', '', '', '', 0, 'id="cpdcpf"', '', $cpdcpf, "pegaNome(this)", null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> Nome: </td>
            <td colspan="2">
                <?PHP
                    $cpdnome = $dados['cpdnome'];
                    echo campo_texto('cpdnome', 'S', 'S', 'Nome', 59, 100, '', '', '', '', 0, 'id="cpdnome"', '', $cpdnome, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Cargo: </td>
            <td colspan="2">
               <?PHP
                    $cpdcargo = $dados['cpdcargo'];
                    echo campo_texto('cpdcargo', 'S', 'S', 'Cargo', 29, 20, '', '', 'left', '', 0, 'id="cpdcargo"', '', $cpdcargo, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone Comercial: </td>
            <td colspan="2">
               <?PHP
                    $textoDica = 'O telefone é composto por código de área (DDD) - Prefixo - Número do telefone. Ex: 61-2200-1111.';

                    $cpdtelefonecomercial = $dados['cpdtelefonecomercial'];
                    echo campo_texto('cpdtelefonecomercial', 'S', 'S', 'Telefone Comercial', 29, 12, '##-####-####', '', 'left', '', 0, 'id="cpdtelefonecomercial"', '', $cpdtelefonecomercial, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone Celular: </td>
            <td colspan="2">
               <?PHP
                    $cpdtelcelular = $dados['cpdtelcelular'];
                    echo campo_texto('cpdtelcelular', 'S', 'S', 'Telefone Celular', 29, 12, '##-####-####', '', 'left', '', 0, 'id="cpdtelcelular"', '', $cpdtelcelular, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> E-mail Institucional: </td>
            <td colspan="2">
                <?PHP
                    $cpdemail = $dados['cpdemail'];
                    echo campo_texto('cpdemail', 'S', 'S', 'E-mail', 59, 100, '', '', '', '', 0, 'id="cpdemail"', '', $cpdemail, '', null);
                ?>
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td width="80%" class="SubTituloCentro" colspan="4" style="font-weight: bold">
            	
            	<?PHP
            		$perfil = pegaPerfilGeral();
                	$docid = criaDocidMantenedora( $_SESSION['maismedicomec']['mntid'] );
                	$esdid = pegaEstadoAtualWorkflow( $docid );
                
                    if( in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil) || in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) ){
                ?>
	                    <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosCorpoDirigente();"/>
                		<input type="button" id="novo" name="novo" value="Novo" onclick="resetFromulario2();"/>
                <?PHP
                    }elseif( in_array(PERFIL_IES, $perfil) && ($esdid == WF_EM_PREENCHIMENTO_IES || $esdid == WF_EM_AJUSTE_IES) ){
                    	if(date('Ymd') < 20150124){
	                    	?>
	                    	<input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosCorpoDirigente();"/>
	                		<input type="button" id="novo" name="novo" value="Novo" onclick="resetFromulario2();"/>
	                    	<?
						}
                    }
                ?>
            	
                <input type="button" id="cancelar" name="cancelar" value="Cancelar" onclick="resetFromulario2();"/>
                <input type="button" id="cancelar" name="voltar" value="Voltar" onclick="voltarPaginaPrincipal();"/>
            </td>
        </tr>
    </table>
</form>

<br>

<?PHP
    $acao = "
		        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscarDadosEditarCorpoDirigente('||c.cpdid||');\" title=\"Editar Professor\" >
		        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDadosMantidaCorpoDirigente('|| c.cpdid ||');\" title=\"Editar Professor\" >
		    ";
    
	if( in_array(PERFIL_IES, $perfil) && ($esdid != WF_EM_PREENCHIMENTO_IES && $esdid != WF_EM_AJUSTE_IES) || (date('Ymd') > 20150123) ){
   		 $acao = "
		        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscarDadosEditarCorpoDirigente('||c.cpdid||');\" title=\"Editar Professor\" >
			    ";
    }

    $sql = "
        SELECT  DISTINCT '{$acao}',
                trim( replace( to_char( cast(c.cpdcpf as bigint), '000:000:000-00' ), ':', '.' ) ) AS cpdcpf,
                c.cpdnome,
                c.cpdtelefonecomercial,
                c.cpdtelcelular,
                c.cpdemail,
                c.cpdcargo,
                CASE WHEN dmd.dmdid IS NOT NULL and md.mtddsc IS NOT NULL and (select count(mnmid) from maismedicomec.mantidamunicipio where mntid = {$_SESSION['maismedicomec']['mntid']}) > 0 THEN md.mtddsc
		     		 WHEN dmd.dmdid IS NOT NULL and md2.mtddsc IS NOT NULL and (select count(mnmid) from maismedicomec.mantidamunicipiocandidata where mntid = {$_SESSION['maismedicomec']['mntid']}) > 0 THEN md2.mtddsc
                     ELSE '-'
                END AS mtddsc

        FROM maismedicomec.corpodirigente AS c

        LEFT JOIN maismedicomec.dirigentemantenedora AS dmt ON dmt.cpdid = c.cpdid
        LEFT JOIN maismedicomec.dirigentemantida AS dmd ON dmd.cpdid = c.cpdid

        LEFT JOIN maismedicomec.mantenedora AS mt ON mt.mntid = dmt.mntid

        LEFT JOIN maismedicomec.mantida AS md ON md.mtdid = dmd.mtdid AND md.mtdstatus = 'A'
        LEFT JOIN maismedicomec.mantidamunicipio AS mm ON mm.mntid = mt.mntid OR mm.mtdid = md.mtdid

        LEFT JOIN maismedicomec.mantidacandidata AS md2 ON md2.mtdid = dmd.mtdid AND md2.mtdstatus = 'A'
        LEFT JOIN maismedicomec.mantidamunicipiocandidata AS mm2 ON mm2.mntid = mt.mntid OR mm2.mtdid = md2.mtdid
        
        WHERE c.cpdstatus='A' and (mm.mntid = {$_SESSION['maismedicomec']['mntid']} OR mm2.mntid = {$_SESSION['maismedicomec']['mntid']})

        ORDER BY 8
    ";
    
    $cabecalho = array("Ação", "CPF", "Nome", "Tel.Comercial", "Tel. Celular", "E-mail", "Cargo", "Mantida");
    $alinhamento = Array('center', '', '', '', '','','','','');
    $tamanho = Array('4%', '', '', '', '', '','','','');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

?>


 
 <script>
 
		function pegaNome(obj) {
                var cpf = obj.value;
                var nome = document.getElementById('cpdnome');
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
