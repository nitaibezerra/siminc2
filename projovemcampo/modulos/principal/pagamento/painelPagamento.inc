<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "projovemcampo/modulos/principal/pagamento/projovemcampo_efetuar_pagamentos.php";
include_once APPRAIZ . "projovemcampo/modulos/principal/pagamento/consultar_pagamentos.php";
// monta cabeçalho
function mostraHistoricoPagamento() {
	global $db;
	array($_POST);

	$sql = "SELECT 
                *
            FROM
            (
            SELECT 
                ( SELECT
                    shtdescricao
                FROM
                    projovemcampo.statushistorictransferencia
                WHERE
                    shtid = (SELECT shtid_status 
                        FROM projovemcampo.historico_transferencia
                        WHERE htrid = (SELECT max(htrid) FROM projovemcampo.historico_transferencia WHERE traid = htr.traid AND htrid < htr.htrid )
                        ) ) as anterior,
                shtdescricao as atual,
                usu.usunome,
                to_char( htrdatahoraacao, 'DD/MM/YYYY HH:MM:SS' ) as data,
                justificativa
            FROM 
                projovemcampo.historico_transferencia htr
            INNER JOIN projovemcampo.statushistorictransferencia   sht ON sht.shtid  = htr.shtid_status
            LEFT  JOIN seguranca.usuario                usu ON usu.usucpf = usucpf_fezacao
            WHERE 
                traid = {$_POST['traid'] }
            ) asfoo
            WHERE
                anterior IS NOT NULL";
	// ver($sql,d);
	$cabecalho = array (
			"Data Envio",
			"Responsável",
			"Quem fez?",
			"Quando fez?",
			"Justificativa" 
	);
	$db->monta_lista ( $sql, $cabecalho, 100, 5, 'N', 'center', '', 'form_transferencias' );
}

if($_REQUEST['requisicao']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';

$check = '<input type="checkbox" id="marcar-todos" >';

echo "<br>";

$perfis = pegaPerfilGeral();

if(in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)|| in_array(PFL_ADMINISTRADOR, $perfis)) {
	$menu = array(0 => array("id" => 1, "descricao" => "Painel", 			"link" => "/projovemcampo/projovemcampo.php?modulo=inicio&acao=C"),
				  1 => array("id" => 2, "descricao" => "Painel Pagamento", 	"link" => "/projovemcampo/projovemcampo.php?modulo=principal/pagamento/painelPagamento&acao=A")
			  	  );
}
else{
	$menu = array(0 => array("id" => 1, "descricao" => "Documentos", 	"link" => "/projovemcampo/projovemcampo.php?modulo=principal/anexoGeral&acao=A")
			  	  );
}


echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

monta_titulo('Painel de Pagamento','');

echo "<br>";
$db->cria_aba( $abacod_tela, $url, '');


//pega array de perfis
//$arPerfil = pegaPerfil( $_SESSION['usucpf'] );

				
if(in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)|| in_array(PFL_ADMINISTRADOR, $perfis)) {
?>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script language="javascript" type="text/javascript">
	jQuery(document).ready(function() {
		jQuery('#marcar-todos').live('click',function(){
	        jQuery('.doc').attr('checked', jQuery(this).attr('checked'));
	    });
		jQuery('#enviarPagamento').click(function(){
	
			if(jQuery('[name="perid[]"]:checked').length<1){
	        	alert('Escolha ao menos um periodo.');
					return false;
	        }
			
			jQuery('#requisicao').val('enviaPagamentoCampo');
			jQuery('#formulario').submit();
		});
		jQuery('#colsultarPagamento').click(function(){
			jQuery('#requisicao').val('consultaPagamentoCampo');
			jQuery('#formulario').submit();
		});
		jQuery('.detalhe').live('click',function(){
			
	        var periodo = jQuery(this).attr('id');

	        jQuery.ajax({
	            type: "POST",
	            url: window.location.href,
	            data: "&requisicao=mostraHistoricoPagamento&perid="+periodo,
	            async: false,
	            success: function(msg){

	                jQuery( "#dialog-detalhe" ).html(msg);       
	                jQuery( "#dialog-detalhe" ).dialog({
	                    resizable: false,
	                    height:500,
	                    width:800,
	                    modal: true,
	                    show: { effect: 'drop', direction: "up" },
	                    buttons: {
	                        "OK": function() {
	                            jQuery( this ).dialog( "close" );                            
	                        }
	                    }
	                });
	            }
	        });
	    });
	});
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
	    var div_on = document.getElementById( campo + '_campo_on' );
	    var div_off = document.getElementById( campo + '_campo_off' );
	    var input = document.getElementById( campo + '_campo_flag' );
	    if ( div_on.style.display == 'none' )
	    {
	        div_on.style.display = 'block';
	        div_off.style.display = 'none';
	        input.value = '1';
	    }
	    else
	    {
	        div_on.style.display = 'none';
	        div_off.style.display = 'block';
	        input.value = '0';
	    }
	}

</script>
<style>
.div_muda{
align:top;
}
</style>
<div id="dialog-detalhe"></div>
<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
<input type="hidden" value="" name="requisicao" id="requisicao" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" width="50%">
		<tr>
   			<td class="SubtituloCentro" colspan="3">
				Não enviados
			</td>    			
		</tr>
		<tr>
			<td width="25%">
			</td>
			<td width="50%" align="center">
				<?
    				$sql="with 
							tmp_pagamentos as  (
						             SELECT count(*) qtd_pagamento, perid, esdid
							     FROM
								     	projovemcampo.lancamentodiario lnd
									 INNER JOIN projovemcampo.estudante est ON est.estid = lnd.estid AND est.cadastradosgb='t'
								     INNER JOIN projovemcampo.diario dia2 ON dia2.diaid = lnd.diaid
								     INNER JOIN projovemcampo.historico_diario hid ON hid.hidid = dia2.hidid AND hid.stdid = 8 
								     INNER JOIN workflow.documento doc3 ON doc3.docid = lnd.docid
							     WHERE esdid in( 1582, 1585 )
							     group by perid, esdid
							 )
						     
						SELECT DISTINCT
						     '<center><input type=\"checkbox\" class=\"doc\" name=\"perid[]\" value=\"'|| dia.perid||'\" /></center>' as acao,
						     perdescricao as periodo, aut.qtd_pagamento as autorizados, rej.qtd_pagamento as rejeitados
						FROM 
						     projovemcampo.lancamentodiario lnd
						INNER JOIN projovemcampo.estudante est ON est.estid = lnd.estid
						INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid AND (diatempoescola is not null AND diatempocomunidade is not null) AND (diatempoescola != 0 AND diatempocomunidade != 0)
						INNER JOIN projovemcampo.historico_diario hid ON hid.hidid = dia.hidid AND hid.stdid = 8 
						INNER JOIN projovemcampo.periodo per ON dia.perid = per.perid
						INNER JOIN projovemcampo.rangeperiodo rap ON rap.rapid = dia.rapid
						INNER JOIN workflow.documento doc ON doc.docid = lnd.docid
						INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
						INNER JOIN projovemcampo.agenciabancariaescola abe ON abe.entid = tur.entid AND agbcod is not NULL 
						left join tmp_pagamentos aut on aut.perid = dia.perid and aut.esdid = 1582
						left join tmp_pagamentos rej on rej.perid = dia.perid and rej.esdid = 1585
						WHERE
						     lnd.docid is not null
						AND doc.esdid in(1582,1585)
						AND (((lndhorasescola + lndhorascomunidade)*100)/(diatempoescola + diatempocomunidade))>= 75 
						AND est.cadastradosgb='t'
						Order By
						     acao
		
						";
//     				ver($sql,d);
    					$tamanho		= array( '20%','40%','20%','20%' );
    					$alinhamento	= array( 'center','center','center','center' );
						$cabecalho = array( $check,"Período" , "Autorizados", "Rejeitados");
						$db->monta_lista( $sql, $cabecalho, 50, 10, 'N','center', '','',$tamanho,$alinhamento);
    			?>
			</td>
			<td width="25%">
			</td>
		</tr>
		<tr>
			<td class="SubtituloCentro" colspan="3">
				<input type="button" name="botao" class="botao" value="Enviar" id="enviarPagamento" />
			</td>
		</tr>
			<?if(1==1){?>
		<tr>
			<td class="SubtituloCentro" colspan="3">
				<input type="button" name="botao" class="botao" value="Colsultar" id="colsultarPagamento" />
		</td>
	</tr>
		<?}?>
	</table>
	<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" width="50%">
		<tr>
			<td class="SubtituloCentro" colspan="3">
				Enviados
			</td>    			
		</tr>
		<tr>
			<td width="25%">
			</td>
			<td width="50%" align="center">
				<?
    				$sql="with 
							tmp_pagamentos as  (
						             SELECT count(*) qtd_pagamento, perid, esdid
							     FROM
								     	projovemcampo.lancamentodiario lnd
									 INNER JOIN projovemcampo.estudante est ON est.estid = lnd.estid AND est.cadastradosgb='t'
								     INNER JOIN projovemcampo.diario dia2 ON dia2.diaid = lnd.diaid
								     INNER JOIN projovemcampo.historico_diario hid ON hid.hidid = dia2.hidid AND hid.stdid = 8 
								     INNER JOIN workflow.documento doc3 ON doc3.docid = lnd.docid
							     WHERE esdid in( 1583, 1584 )
							     group by perid, esdid
							 )
						     
						SELECT DISTINCT
						     perdescricao as periodo, 
							aut.qtd_pagamento as enviados, 
							rej.qtd_pagamento as pagos,
							'<div class=detalhe id='|| per.perid ||' >
				                <img style=cursor:pointer width=55px src=/imagens/gadget_busca.png border=0 />
				            </div>' as detalhe
						FROM 
						     projovemcampo.lancamentodiario lnd
						INNER JOIN projovemcampo.estudante est ON est.estid = lnd.estid
						INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid AND (diatempoescola is not null AND diatempocomunidade is not null) AND (diatempoescola != 0 AND diatempocomunidade != 0)
						INNER JOIN projovemcampo.historico_diario hid ON hid.hidid = dia.hidid AND hid.stdid = 8 
						INNER JOIN projovemcampo.periodo per ON dia.perid = per.perid
						INNER JOIN projovemcampo.rangeperiodo rap ON rap.rapid = dia.rapid
						INNER JOIN workflow.documento doc ON doc.docid = lnd.docid
						INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
						INNER JOIN projovemcampo.agenciabancariaescola abe ON abe.entid = tur.entid AND agbcod is not NULL 
						left join tmp_pagamentos aut on aut.perid = dia.perid and aut.esdid = 1583
						left join tmp_pagamentos rej on rej.perid = dia.perid and rej.esdid = 1584
						WHERE
						     lnd.docid is not null
						AND doc.esdid in(1583,1584)
						AND (((lndhorasescola + lndhorascomunidade)*100)/(diatempoescola + diatempocomunidade))>= 75 
						AND	est.cadastradosgb='t'
						Order By
						     periodo
		
						";
//     				ver($sql,d);
    					$tamanho		= array( '25%','25%','25%','25%' );
    					$alinhamento	= array( 'center','center','center','center','center' );
						$cabecalho = array( "Período" , "Enviados", "Pagos", "Histórico");
						$db->monta_lista( $sql, $cabecalho, 50, 10, 'N','center', '','',$tamanho,$alinhamento);
    			?>
			</td>
			<td width="25%">
			</td>
		</tr>
	</table>
</form>
<?

} // FECHA 

