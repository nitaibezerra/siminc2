<?php

verificaMuncod();

// Includes necessários
include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/cabecalho.inc';
include_once APPPISO . 'classes/FolhaPagamento.class.inc';

// Objetos
$obFolhaPagamento = new FolhaPagamento();

// Monta abas
print '<br/>';
$abacod_tela = '57365';
$arMnuid = array(ABA_FP_DUPLICAR_MES, ABA_FP_LISTAR_PROFIS, ABA_FP_FORM_PROFISSIONAL, ABA_FP_IMPORTAR_ARQUIVO);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// Monta cabeçalhos
monta_titulo('Complementação do Piso Salarial', 'Folha de Pagamento');
montaCabecalho();

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
$(document).ready(function(){

    // Botões de ações das folhas de pagamento
    $('.acao_folha_pagamento').click(function(){

        arIds  = this.id.split('-');
        pagina = arIds[0];
        mesref = arIds[1];
        anoref = arIds[2];

        $('input[name=mesref]').val(mesref);
        $('input[name=anoref]').val(anoref);
        $('input[name=pagina]').val(pagina);

        if(pagina != 'fecharFolha' && pagina != 'abrirFolha'){

            $('#formulario').attr('action', 'pisosalarial.php?modulo=principal/folhapagamento/'+pagina+'&acao=A');
            $('#formulario').submit();

        }else{

            if(pagina == 'fecharFolha'){

                if(confirm('Folha de Pagamento: '+mesref+'/'+anoref+'\n\nApós o fechamento da Folha de Pagamento o sistema não permitirá mais inclusão ou alteração dos profissionais. Confirma fechamento?')){

                    jQuery.ajax({
                        url: 'ajax.php',
                        type: 'post',
                        data: 'requisicao=finalizaFolhaPagamento&flpanoreferencia='+anoref+'&flpmesreferencia='+mesref,
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
            }

            if(pagina == 'abrirFolha'){

                if(confirm('Deseja abrir a folha de pagamento '+mesref+'/'+anoref+'?')){
                    jQuery.ajax({
                        url: 'ajax.php',
                        type: 'post',
                        data: 'requisicao=finalizaFolhaPagamento&acao=abrir&flpanoreferencia='+anoref+'&flpmesreferencia='+mesref,
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
            }
        }
    });
});
</script>
<style>
<!--
.acao_folha_pagamento{
    cursor:pointer;
}
.listagem{
	width: 100%;
}
-->
</style>
<table class="tabela" align="center" width="95%" bgcolor="" border="0" cellpadding="5" cellspacing="1">
	<tr bgcolor="">
		<td style="width: 100%; text-align: left;">
		<table width="100%" style="text-align: left;" border="0" cellpadding="0" cellspacing="0">
			<tr>
				<td style="text-align: left;" width="100%"><? $obFolhaPagamento->recuperarListaFolhasPagamento(); ?></td>
			</tr>
		</table>
		</td>
		<td valign="top" style="width: 80px"><?
					if( !empty( $_SESSION['piso']['muncod'] ) ){
						$docid = criarDocumento( $_SESSION['piso']['muncod'] );
						wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'muncod' => $_SESSION['piso']['muncod'] ) );
					} ?></td>
	</tr>
</table>

<form id="formulario" method="post" action="">
    <input type="hidden" name="mesref" value="" />
    <input type="hidden" name="anoref" value="" />
    <input type="hidden" name="pagina" value="" />
    <input type="hidden" name="muncod" value="<?php echo $_SESSION['piso']['muncod'] ?>" />
</form>