<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<?php

verificaMuncod();

// Includes necessarios
include_once APPRAIZ . 'includes/cabecalho.inc';
include_once APPPISO . 'classes/FolhaPagamento.class.inc';
include_once APPPISO . 'classes/FolhaPagamentoProfissionais.class.inc';

// Objetos
$obFolhaPagamento = new FolhaPagamento();
$obFolhaProfissionais = new FolhaPagamentoProfissionais();

if($_POST['requisicao'] == 'salvar'){

    $obFolhaProfissionais->salvarProfissionais($_REQUEST);
    echo "<script>
            $(document).ready(function(){
                alert('Operação realizada com sucesso.');
                $('input[name=requisicao]').val('');
                $('#formulario').attr('action', 'pisosalarial.php?modulo=principal/folhapagamento/listaProfissionais&acao=A');
                $('#formulario').submit();
            });
          </script>";
//    header("Location: ?modulo=principal/folhapagamento/listaFolhaPagamento&acao=A");
}

// Monta abas
print '<br/>';
$abacod_tela = '57365';
$arMnuid = array(ABA_FP_DUPLICAR_MES, ABA_FP_IMPORTAR_ARQUIVO, ABA_FP_FORM_PROFISSIONAL);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// Monta cabeçalhos
monta_titulo('Lista de Profissionais', "Folha de Pagamento");
montaCabecalho();

$esdid = pegarEstadoAtual($_SESSION['piso']['muncod']);
?>

<script type="text/javascript">

$(document).ready(function(){

    // Botão fechar folha de pagamento
    $('.fecharFolha').click(function(){
        if(confirm('Folha de Pagamento: '+$('input[name=mesref]').val()+'/'+$('input[name=anoref]').val()+'\n\nApós o fechamento da Folha de Pagamento o sistema não permitirá mais inclusão ou alteração dos profissionais. Confirma fechamento?')){
            jQuery.ajax({
                url: 'ajax.php',
                type: 'post',
                data: 'requisicao=finalizaFolhaPagamento&flpanoreferencia='+$('input[name=anoref]').val()+'&flpmesreferencia='+$('input[name=mesref]').val(),
                success: function(e){
                    if(e == 'true'){
                        document.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/listaFolhaPagamento&acao=A';
                    }
                    if(e == 'sem'){
                        alert('Não existem profissionais para folha de pagamento selecionada.\nOperação cancelada.');
                    }
                }
            });
        }
    });

    // Botão editar profissional
    jQuery('.btEditar').click(function(){

        jQuery('input[name=fppid]').val(this.id);
        jQuery('input[name=requisicao]').val('editar');

        form = jQuery('#formulario');
        form.attr('action','pisosalarial.php?modulo=principal/folhapagamento/formProfissional&acao=A');
        form.submit();

    });

    // Visualizar detalhes do profissional
    jQuery('.btVisualizar').click(function(){

        jQuery('input[name=fppid]').val(this.id);
        jQuery('input[name=requisicao]').val('visualiza_profissional');

        form = jQuery('#formulario');
        form.attr('action','pisosalarial.php?modulo=principal/folhapagamento/listaProfissionais&acao=A');
        form.submit();
    });

    // Botão excluir profissional
    jQuery('.btExcluir').click(function(){
        if(confirm('Deseja excluir o registro?')){
            jQuery.ajax({
                url: 'ajax.php',
                data: 'requisicao=deletarProfissionalFolha&fppid='+this.id,
                type: 'post',
                success: function(e){

                }
            });
            jQuery(this).parent().parent().parent().remove();
        }
    });

    // Botão novo profissional
    $('.novoProfissional').click(function(){
        $('#formulario').attr('action', 'pisosalarial.php?modulo=principal/folhapagamento/formProfissional&acao=A');
        $('#formulario').submit();
    });

    // Botão voltar
    $('.voltar').click(function(){
        if(this.id == 'lista'){
            $('input[name=requisicao]').val('');
            $('#formulario').attr('action', 'pisosalarial.php?modulo=principal/folhapagamento/listaProfissionais&acao=A');
            $('#formulario').submit();
        }else{
            document.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/listaFolhaPagamento&acao=A';
        }
    });

    // Abre a folha de pagamento
    $('.abrirFolha').click(function(){
        if(confirm('Deseja abrir a folha de pagamento '+$('input[name=mesref]').val()+'/'+$('input[name=anoref]').val()+'?')){
            jQuery.ajax({
                url: 'ajax.php',
                type: 'post',
                data: 'requisicao=finalizaFolhaPagamento&acao=abrir&flpanoreferencia='+$('input[name=anoref]').val()+'&flpmesreferencia='+$('input[name=mesref]').val(),
                success: function(e){

                    if(e == 'true'){
                        document.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/listaFolhaPagamento&acao=A';
                    }

                    if(e == 'sem'){
                        alert('Não existem profissionais para folha de pagamento selecionada.\nOperação cancelada.');
                    }
                }
            });
        }
    });

    // Formata total das cargas horárias da lista
    if($('input[name=requisicao]').val() != 'visualiza_profissional'){
        carga_horaria = $('.listagem:first').find('tr:last').find('td:last').html().split(',');
        $('.listagem:first').find('tr:last').find('td:last').html(replaceAll(carga_horaria[0], '.', ''));
    }

    //botão limpar
    $('.limpar').click(function(){
    	$('input[name=pcpf]').val('');
    	$('input[name=pnome]').val('');
        $('select[name=pcargo]').val('');
        $('#formulario').attr('action', 'pisosalarial.php?modulo=principal/folhapagamento/listaProfissionais&acao=A');
        $('#formulario').submit();
    });

    // botão pesquisar
    $('.pesquisar').click(function(){

		/*
        if($('input[name=pcpf]').val() == '' && $('input[name=pnome]').val() == '' && $('select[name=pcargo]').val() == ''){
            alert('Informe o CPF ou Nome ou Cargo para pesquisar!');
            $('input[name=pcpf]').focus();
            return false;
        }
        */
                
        $('#formulario').append('<input type="hidden" name="fppcpf" value="'+$('input[name=pcpf]').val()+'" />');
        $('#formulario').append('<input type="hidden" name="fppnome" value="'+$('input[name=pnome]').val()+'" />');
        $('#formulario').append('<input type="hidden" name="fppcargo" value="'+$('select[name=pcargo]').val()+'" />');
        $('input[name=requisicao]').val('');
        $('#formulario').submit();

    });    
    

    $('.salvar').click(function(){

        if($('select[name=anodup]').val() == ''){
            alert('Selecione um ano para duplicar!');
            $('select[name=anodup]').focus();
            return false;
        }
        if($('select[name=mesdup]').val() == ''){
            alert('Selecione um mês para duplicar!');
            $('select[name=mesdup]').focus();
            return false;
        }

        $.each($('input[name=fppid[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="fppid[]" value="'+obj.value+'" />');
        });
        $.each($('input[name=fppsalbase[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="fppsalbase['+obj.id.replace('fppsalbase_', '')+']" value="'+obj.value+'" />');
        });
        $.each($('input[name=fppadigrat[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="fppadigrat['+obj.id.replace('fppadigrat_', '')+']" value="'+obj.value+'" />');
        });
        $.each($('input[name=fppcargahoraria[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="fppcargahoraria['+obj.id.replace('fppcargahoraria_', '')+']" value="'+obj.value+'" />');
        });
        $.each($('select[name=carid[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="carid['+obj.id.replace('carid_', '')+']" value="'+obj.value+'" />');
        });
        $.each($('select[name=sitid[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="sitid['+obj.id.replace('sitid_', '')+']" value="'+obj.value+'" />');
        });

        campos = $('#formulario').serialize();
        $.ajax({
            url: 'ajax.php',
            type: 'post',
            data: 'requisicao=verificaAntesDuplicar&'+campos,
            success: function(e){
                if(e > 0){
                    if(!confirm('Está operação irá sobrescrever CPF já existentes no mês selecionado. Deseja continuar?')){
                        return false;
                    }
                }
                $('input[name=requisicao]').val('salvar');
                $('#formulario').submit();
            }
        });
    });

    fppid = $('input[name=fppid[]]');

    carid = $('select[name=carid[]]');
    $('input[name=carid_temp[]]').each(function(key, obj){
        carid[key].value = obj.value;
        carid[key].id = 'carid_'+fppid[key].value;
    });
    sitid = $('select[name=sitid[]]');
    $('input[name=sitid_temp[]]').each(function(key, obj){
        sitid[key].value = obj.value;
        sitid[key].id = 'sitid_'+fppid[key].value;
    });

    $('.monetario').change(function(){
        arId = this.id.split('_');

        fppsalbase = parseFloat(replaceAll(replaceAll($('#fppsalbase_'+arId[1]).val(), '.', ''), ',', '.'));
        fppadigrat = parseFloat(replaceAll(replaceAll($('#fppadigrat_'+arId[1]).val(), '.', ''), ',', '.'));

        if(fppsalbase > 0 && fppadigrat > 0){
            fppcalculo = fppsalbase+fppadigrat;
            $('#fppcalculo_'+arId[1]).val(mascaraglobal('###.###.###.###,##', fppcalculo.toFixed(2)));
        }
    });
});
</script>


<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
    	<tr>
    		<td class="SubtituloDireita" style="width: 19%">CPF:</td>
    		<td>
    			<?$pcpf = $_REQUEST['fppcpf'];?>
    			<?=campo_texto( 'pcpf', 'N', 'S', '', 16, 14, '###.###.###-##', '', '', '', 0, 'id=pcpf');?>
    		</td>
    	</tr>
    	<tr>
    		<td class="SubtituloDireita">Nome:</td>
    		<td>
    			<?$pnome = $_REQUEST['fppnome'];?>
    			<?=campo_texto( 'pnome', 'N', 'S', '', 60, 60, '', '', '', '', 0, '');?>
    		</td>
    	</tr>
    	<tr>
    		<td class="SubtituloDireita" >Cargo:</td>
    		<td><?
    			$pcargo = $_REQUEST['fppcargo'];
    			
    			if($_REQUEST['mesref'] && $_REQUEST['anoref']) $andmesano = " AND f.flpmesreferencia = '".$_REQUEST['mesref']."'  AND f.flpanoreferencia = '".$_REQUEST['anoref']."' ";
    			
				$sql_combo = "SELECT c.carid as codigo, c.cardsc as descricao 
							  FROM pisosalarial.cargo c
							  WHERE c.carstatus = 'A' 
							  and c.carid in (select distinct carid 
												from pisosalarial.folhapagamentoprofissionais fp
												inner join pisosalarial.folhapagamento f on f.flpid = fp.flpid
												inner join pisosalarial.pisomunicipio p on p.pmuid = f.pmuid and p.muncod = '".$_SESSION['piso']['muncod']."' $andmesano)
							  ORDER BY c.cardsc";
				$db->monta_combo("pcargo", $sql_combo, 'S', "Selecione o Cargo", '', '', '', '', 'N', 'pcargo');
				?>
			</td>
    	</tr>
    	<tr>
    		<td class="SubtituloDireita" ></td>
    		<td class="SubtituloEsquerda">
    			<input class="pesquisar" type="button" value="Pesquisar" />
    			&nbsp;
    			<input class="limpar" type="button" value="Limpar" />
    		</td>
    	</tr>
</table>

<?php 
if($_REQUEST['requisicao'] == 'visualiza_profissional'){
    include_once APPPISO . 'modulos/principal/folhapagamento/detalheProfissional.inc';
}else{
	?>
        <table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
            <tr bgcolor="#cccccc">
               <td align="right">
                <?php if(!in_array($esdid, array(WF_APROVADO))): ?>
                    <?php if($obFolhaPagamento->verificaFolhaFinalizada($_SESSION['piso']['muncod'], $_REQUEST['anoref'], $_REQUEST['mesref']) && possuiPerfil(array(PERFIL_CADASTRO_MUNICIPAL))): ?>
                        <input class="salvar" type="button" value="Salvar" />
                        <input class="novoProfissional" type="button" value="Novo profissional" />
                        <input class="fecharFolha" type="button" value="Fechar folha de pagamento" />
                    <?php elseif(!$obFolhaPagamento->verificaFolhaFinalizada($_SESSION['piso']['muncod'], $_REQUEST['anoref'], $_REQUEST['mesref']) && possuiPerfil(array(PERFIL_CADASTRO_MUNICIPAL))): ?>
                        <input class="abrirFolha" type="button" value="Abrir folha de pagamento" />
                    <?php endif; ?>
                <?php endif; ?>
                <input class="voltar" type="button" value="Voltar" />
               </td>
            </tr>
        </table>
	<?php 
	$obFolhaProfissionais->listaDadosProfissionais($_REQUEST, $regPorPag = 50);
    ?>
        <table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
            <tr bgcolor="#cccccc">
               <td align="right">
                <?php if(!in_array($esdid, array(WF_APROVADO))): ?>
                    <?php if($obFolhaPagamento->verificaFolhaFinalizada($_SESSION['piso']['muncod'], $_REQUEST['anoref'], $_REQUEST['mesref']) && possuiPerfil(array(PERFIL_CADASTRO_MUNICIPAL))): ?>
                        <input class="salvar" type="button" value="Salvar" />
                        <input class="novoProfissional" type="button" value="Novo profissional" />
                        <input class="fecharFolha" type="button" value="Fechar folha de pagamento" />
                    <?php elseif(!$obFolhaPagamento->verificaFolhaFinalizada($_SESSION['piso']['muncod'], $_REQUEST['anoref'], $_REQUEST['mesref']) && possuiPerfil(array(PERFIL_CADASTRO_MUNICIPAL))): ?>
                        <input class="abrirFolha" type="button" value="Abrir folha de pagamento" />
                    <?php endif; ?>
                <?php endif; ?>
                <input class="voltar" type="button" value="Voltar" />
               </td>
            </tr>
        </table>
<?php
}
?>
<form id="formulario" method="post" action="">
    <input type="hidden" name="fppid" value="" />
    <input type="hidden" name="requisicao" id="requisicao" value="<?php echo $_REQUEST['requisicao'] ? $_REQUEST['requisicao'] : ''?>">
    <input type="hidden" name="mesref" value="<?php echo $_REQUEST['mesref'] ?>" />
    <input type="hidden" name="anoref" value="<?php echo $_REQUEST['anoref'] ?>" />
    <input type="hidden" name="muncod" value="<?php echo $_SESSION['piso']['muncod'] ?>" />
    <input type="hidden" name="fppsalbase" value="" />
    <input type="hidden" name="fppadigrat" value="" />
</form>