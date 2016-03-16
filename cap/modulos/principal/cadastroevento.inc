<?php
$perfil = pegarPerfil($_SESSION['usucpf']);

if($_REQUEST['requisicao']=='cadastrar'){
	salvarEvento($_POST);
	exit();
}

if($_REQUEST['requisicao']=='alterar'){
	alterarEvento($_POST,$perfil);
	exit();
}

if(in_array(CAP_PERFIL_SUPER_USUARIO,$perfil) || in_array(CAP_PERFIL_ADMINISTRADOR,$perfil)){
	$modulo = "Módulo Administrativo";
} else {
	$modulo = "Módulo Servidor";
}

if(isset($_REQUEST['aftid'])){
	$aryDados = exibirDadosEvento($_REQUEST['aftid']);
	extract($aryDados);
}

$titulo_modulo = "Afastamento do País";
monta_titulo( $titulo_modulo, $modulo);
?>

<br>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>

<script type="text/javascript">
    jQuery.noConflict();

    function validarEvento(){
        if(jQuery('[name=paiid]').val() == ''){
                alert('O campo "País" é obrigatório!');
                jQuery('[name=paiid]').focus();
                return false;
        }
        if(jQuery('[name=cdpdsc]').val() == ''){
                alert('O campo "Cidade" é obrigatório!');
                jQuery('[name=cdpdsc]').focus();
                return false;
        }
        if(jQuery('[name=aftdtinicio]').val() == '' || jQuery('[name=aftdtfinal]').val() == ''){
                alert('O campo "Período" é obrigatório!');
                jQuery('[name=aftdtinicio]').focus();
                return false;
        }

        //Período com Trânsito Incluso
        dtini = window.opener.document.getElementById('afppertraninicial').value;
        if(dtini){
                d1ini = dtini.substr(6,4) + dtini.substr(3,2) + dtini.substr(0,2);
        }else{
                d1ini = 0;
        }


        dtfim = window.opener.document.getElementById('afppertranfinal').value;
        if(dtfim){
                d1fim = dtfim.substr(6,4) + dtfim.substr(3,2) + dtfim.substr(0,2);
        }else{
                d1ini = 0;
        }

        //Período
        dtini = document.getElementById('aftdtinicio').value;
        if(dtini){
                d2ini = dtini.substr(6,4) + dtini.substr(3,2) + dtini.substr(0,2);
        }else{
                d1ini = 0;
        }

        dtfim = document.getElementById('aftdtfinal').value;
        if(dtfim){
                d2fim = dtfim.substr(6,4) + dtfim.substr(3,2) + dtfim.substr(0,2);
        }else{
                d1ini = 0;
        }

        if(d2ini < d1ini || d2ini > d1fim){
                alert('O campo "Período - Data Início" deve estar dentro do intervalo do "Período com Trânsito Incluso"!');
                jQuery('[name=aftdtinicio]').focus();
                return false;
        }
        if(d2fim < d1ini || d2fim > d1fim){
                alert('O campo "Período - Data Término" deve estar dentro do intervalo do "Período com Trânsito Incluso"!');
                jQuery('[name=aftdtfinal]').focus();
                return false;
        }

        if(jQuery('[name=aftobjetivo]').val() == ''){
                alert('O campo "Objetivo" é obrigatório!');
                jQuery('[name=aftobjetivo]').focus();
                return false;
        }
        return true;
    }

    function salvarEvento(){
        if(validarEvento()){
            jQuery.ajax({
                url: 'cap.php?modulo=principal/cadastroevento&acao=A',
                data: jQuery('#formulario').serialize(),
                async: false,
                type: 'POST',
                success: function(data){
                    if(trim(data) == 'S'){
                        var divEvento = window.opener.document.getElementById('div_evento');
                        if(trim(jQuery('#requisicao').val()) == 'cadastrar'){
                            alert('Evento cadastrado com sucesso!');
                            window.opener.location.href="cap.php?modulo=principal/formulario&acao=A";
                        } else {
                            alert('Evento alterado com sucesso!');
                            window.opener.location.href="cap.php?modulo=principal/formulario&acao=A";
                            window.close();
                        }
                    } else if(trim(data) == 'E') {
                        alert('Não foi possível realizar o cadastro! já existe evento neste período!');
                        location.href = "cap.php?modulo=principal/cadastroevento&acao=A";
                        window.close();
                    } else {
                        location.href = "cap.php?modulo=principal/cadastroevento&acao=A";
                    }
                }
            });
        }
    }
</script>

<?php if(!empty($_SESSION['cap']['afpid'])){ ?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr bgcolor="#CCCCCC">
		<td><b>Servidor:</b>&nbsp;<?php echo exibirNomeServidor($_SESSION['cap']['afpid']); ?></td>
	</tr>
</table>
<br>
<?php } ?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
    <tr>
        <td class="SubTituloEsquerda" colspan="2">País / Evento</td>
    </tr>
</table>

<form id="formulario" method="post" name="formulario" action="#">
	<?php if($aftid){ ?>
	<input type="hidden" id="aftid" name="aftid" value="<?php echo $aftid; ?>"/>
	<input type="hidden" id="requisicao" name="requisicao" value="alterar"/>
	<?php } else { ?>
	<input type="hidden" id="requisicao" name="requisicao" value="cadastrar"/>
	<input type="hidden" id="afpid" name="afpid" value="<?php echo $_SESSION['cap']['afpid']; ?>"/>
	<?php } ?>
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
		<tr>
			<td class="SubTituloDireita" width="20%">País:</td>
			<td>
				<?php $sql_pais = "SELECT idpdr AS codigo, prddsc AS descricao FROM cap.paisdiarias ORDER BY prddsc"; ?>
	        	<?php $db->monta_combo('idpdr',$sql_pais,'S','Selecione...','','','','372','S','','',$idpdr); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Cidade:</td>
			<td><?php echo campo_texto('cdpdsc', 'S', 'S', '', 50, 150, '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Período:</td>
			<td>
				<?php echo campo_data2('aftdtinicio', 'S', 'S', '', 'S', '', '', $dataini); ;?>&nbsp;&nbsp;&nbsp;&nbsp;a
				&nbsp;&nbsp;&nbsp;&nbsp;<?php echo campo_data2('aftdtfinal', 'S', 'S', '', 'S', '', '', $datafim);  ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Atividade:</td>
			<td><?php echo campo_textarea('aftobjetivo', 'S', 'S', '', '80', '5', '255'); ?></td>
		</tr>
		<tr>
			<td align="center" bgcolor="#CCCCCC" colspan="2">
				<input type="button" value="Salvar" id="btnSalvar" onclick="salvarEvento();"/>
				<input type="button" value="Cancelar" id="btnVerTodos" onclick="window.close();"/>
			</td>
		</tr>
	</table>
</form>
<br>