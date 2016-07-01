<?php

verificaCodigoTermoSessao();

#Excluir previsao orcamentaria
if($_REQUEST['requisicao'] == 'excluirPrevisao'){

	if($_REQUEST['id']){
// 		$sql = "delete from monitora.previsaoorcamentaria where proid = {$_REQUEST['id']}";
		$sql = "update monitora.previsaoorcamentaria set prostatus = 'I' where proid = {$_REQUEST['id']}";
		$db->executar($sql);
		if($db->commit()){
			die('1');
		}
	}
	die('0');
}

/**
 * Mostra MODAL com a celula orcamentaria a ser remanejada
 */
if ($_POST['remanejar_id']) {

    $strSQL = "SELECT DISTINCT
            pro.proid,
            ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as ptrid_descricao,
            substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as pliid_descricao,
            substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as ndp_descricao,
            pro.ptrid,
            a.acacod,
            pro.pliid,
            case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acatitulo,
            pro.ndpid,
            to_char(pro.provalor, '999G999G999G999G999D99') as provalor,
            coalesce(pro.provalor, 0) as valor,
            crdmesliberacao,
            crdmesexecucao,
            pro.proid,
            pro.proanoreferencia,
            pro.prodata,
            (select ppa2.codncsiafi from elabrev.previsaoparcela ppa2 where ppa2.ppaid = (select max(ppa1.ppaid) from elabrev.previsaoparcela ppa1 where ppa1.proid = pro.proid) and ppa2.ppacancelarnc = 'f') as lote,
            pp.codsigefnc,
        pp.codncsiafi,
        tc.ungcodconcedente
        FROM monitora.previsaoorcamentaria pro
        LEFT JOIN monitora.pi_planointerno pi 		ON pi.pliid = pro.pliid
        LEFT JOIN monitora.pi_planointernoptres pts ON pts.pliid = pi.pliid
        LEFT JOIN public.naturezadespesa ndp 		ON ndp.ndpid = pro.ndpid
        LEFT JOIN monitora.ptres p 					ON p.ptrid = pro.ptrid
        LEFT JOIN monitora.acao a 					ON a.acaid = p.acaid
        LEFT JOIN public.unidadegestora u 			ON u.unicod = p.unicod
        LEFT JOIN monitora.pi_planointernoptres pt 	ON pt.ptrid = p.ptrid
        LEFT JOIN elabrev.previsaoparcela pp		ON (pp.proid = pro.proid)
        LEFT JOIN monitora.termocooperacao tc       ON (tc.tcpid = pro.tcpid)
        LEFT JOIN public.unidadegestora unc         ON (unc.ungcod = tc.ungcodconcedente)
        WHERE pro.prostatus = 'A'
        AND pro.proid = {$_POST['remanejar_id']}
        ORDER BY lote, pro.proanoreferencia DESC, crdmesliberacao";

    $row = $db->pegaLinha($strSQL);

    $logs = $db->carregar("
        select
            cr.*, to_char(cr.crdata, 'DD/MM/YYYY HH24:MI:SS') as data, s.usunome
        from elabrev.creditoremanejado cr
            inner join seguranca.usuario s using (usucpf)
        where cr.proid = {$_POST['remanejar_id']}
    ");

    echo '<form id="formRemanejar" name="formRemanejar" method="post">
        <table cellspacing="2" cellpadding="2">
            <tr>
                <td colspan="2" align="center">
                    <h2>Remanejamento de crédito</h2>
                    <input type="hidden" name="proid" value="'. $_POST['remanejar_id'] .'" />
                </td>
            </tr>
            <tr>
                <td colspan="2" align="center">
                    <table>
                        <tr>
                            <th>Ano</th>
                            <th>Ação</th>
                            <th>Programa de Trabalho</th>
                            <th>Plano Interno</th>
                            <th>Descrição da Ação</th>
                            <th>Nat. da Despesa</th>
                            <th>Valor original</th>
                            <th>Mês Liberação</th>
                            <th>Prazo cumprimento</th>
                        </tr>
                        <tr>
                            <td>'. $row['proanoreferencia'] .'</td>
                            <td align="center">'. $row['acacod'] .'</td>
                            <td>'. $row['ptrid_descricao'] .'</td>
                            <td>'. $row['pliid_descricao'] .'</td>
                            <td>'. $row['acatitulo'] .'</td>
                            <td>'. $row['ndp_descricao'] .'</td>
                            <td align="center">'. trim($row['provalor']) .'</td>
                            <td align="center">'. mesPorExtenso($row['crdmesliberacao']) .'</td>
                            <td align="center">'. $row['crdmesexecucao'] .' Mês(s) </td>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">NC devoluçao:</td>
                <td align="left">
                '.campo_texto('nc_devolucao','S','S','','40','','','', 'left', '', '', '', '', '').'
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Valor:</td>
                <td align="left">
                <input type="hidden" name="provalor" id="_provalor_" value="'.trim($row['valor']).'" />
                <input type="hidden" name="_proid" id="_proid" value="'.$_POST['remanejar_id'].'" />
                '.campo_texto('valor_remanejar','S','S','','40','','[.###],##','', 'right', '', '', '', '', '').'
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Observação:</td>
                <td align="left">
                '.campo_textarea('observacao', 'N', 'S', '', 65, 3, 300).'
                </td>
            </tr>
            <tr>
                <td colspan="2" align="center">
                    <input type="button" class="fecharBtn" name="fecharBtn" value="Fechar">
                    <input type="button" class="submitBtn" name="submitBtn" value="Remanejar crédito">
                </td>
            </tr>
        </table>
      </form>';

    if ($logs) {
        echo '<table cellspacing="2" cellpadding="2" align="center">
            <tr>
                <th>Valor remanejado:</th>
                <th>Efetuado em:</th>
                <th>NC devolução:</th>
                <th>Observação:</th>
                <th>Usuário/CPF:</th>
            </tr>';
        foreach ($logs as $log) {
            echo '<tr>
                <td align="left">'.number_format($log['valor'], 2, ',', '.').'</td>
                <td align="left">'.$log['data'].'</td>
                <td align="left">'.utf8_decode($log['nc_devolucao']).'</td>
                <td align="left">'.utf8_decode($log['crobservacao']).'</td>
                <td align="left">'.$log['usunome'].' ('.trim($log['usucpf']).')</td>
            </tr>';
        }
        echo '</table>';
    }

    echo '<script type="text/javascript">
        $(".submitBtn").click(function(e){
            e.preventDefault();

            var _formData = $("#formRemanejar").serialize();

            $.ajax({
                url: "/elabrev/elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao"
              , data: _formData+"&valida_operacao=TRUE"
              , type: "post"
              , beforeSend: function(){}
              , success: function(data){
                    $("body").append(data);
                }
              , complete: function(){}
            });
        });
    </script>';
    die;
}

/**
 * Valida e registra um remanejamento de credito
 */
if ($_POST['valida_operacao']) {
    $msg = '';

    if (empty($_POST['nc_devolucao'])) {
        $msg.= 'Preencha o campo NC Devolução. \n';
    }

    if (!preg_match('!^\d{4}(NC|nc)\d{6}$!', $_POST['nc_devolucao'])) {
        $msg.= 'Formato inválido para valor da NC Devolução. \n';
    }

    if (empty($_POST['valor_remanejar'])) {
        $msg.= 'Preencha o campo Valor. \n';
    }

    if ((int)$_POST['valor_remanejar'] == 0) {
        $msg.= 'O valor preenchido precisa ser maior do que zero. \n';
    }

    $valor_remanejar = str_replace('.', '', $_POST['valor_remanejar']);
    $valor_remanejar = str_replace(',', '.', $valor_remanejar);
    $observacao = strip_tags($_POST['observacao']);
    $crobservacao = substr($observacao, 0, 300);
    $nc_devolucao = strip_tags($_POST['nc_devolucao']);
    $result = ($_POST['provalor']-$valor_remanejar);

    if ($valor_remanejar > $_POST['provalor']) {
        $msg.= 'O valor informado é maior que o disponível na celula. \n';
    }

    if (!empty($msg)) {
        die('<script type="text/javascript">alert("'.$msg.'");</script>');
    } else {

        if ($valor_remanejar == $_POST['provalor']) {
            $db->executar("UPDATE monitora.previsaoorcamentaria SET prostatus = 'I', statusvalorzerado = 't' WHERE proid = {$_POST['_proid']}");
        }

        $db->executar("INSERT INTO elabrev.creditoremanejado(proid, valor, nc_devolucao, crdata, crobservacao, usucpf)
                       VALUES ({$_POST['_proid']}, $valor_remanejar, '{$nc_devolucao}', 'NOW()', '{$crobservacao}', '{$_SESSION['usucpf']}')");
        $db->commit();
        alertlocation(array(
            'alert' => 'Credito remanejado com sucesso!'
          , 'location' => '/elabrev/elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao'
        ));
    }
}

$contador = 1;

#Carrega planos de trabalhos autocomplete
if($_REQUEST['requisicao'] == 'carregaPlanosTrabalho'){

	$sql = "SELECT * FROM (
				SELECT DISTINCT
					p.ptrid as codigo,
					ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
				FROM monitora.ptres p
				JOIN public.unidadegestora u
					ON u.unicod = p.unicod
				WHERE p.ptrano = '{$_SESSION['exercicio']}'
				AND p.ptrstatus = 'A'
				AND u.unicod IN ( '26101','26298','26291','26290' )
			) AS foo
			".($_GET["q"] ? "WHERE descricao ILIKE '%{$_GET["q"]}%'" : "")."";

	$arDados = $db->carregar($sql);

	$q = strtolower($_GET["q"]);
	if($arDados){
		foreach ($arDados as $key=>$value){
			echo "{$value['descricao']}|{$value['codigo']}\n";
		}
	}else{
		echo "Sem registros|";
	}
	die;
}

if ($emCadastramentoCorrecao && in_array(UO_EQUIPE_TECNICA, $perfis) && $boEmAlteracao){
	$habilita 				= 'S';
	$habilita_Und_G 		= 'S';
	$habilitaSalvar 		= 'S';
	$habilitaInserir 		= 'S';
	$habilita_Und_G 		= 'S';
	$habilita_pro_con 		= 'S';
}

$arEstadosDocCadNC = array(EM_DESCENTRALIZACAO, 
							EM_EXECUCAO, 
							RELATORIO_OBJ_AGUARDANDO_APROV_GESTOR, 
							RELATORIO_OBJ_AGUARDANDO_APROV_REITORIA,
							RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD,
							TERMO_FINALIZADO);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/jquery.livequery.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
<style>
	.linkLote{
		cursor:pointer;
		color:blue;
	}
	.linkLote:hover{
		cursor:pointer;
		text-decoration:underline;	
		color:blue;
	}
</style>
<script type="text/javascript">
	$(function(){
		$('.linkLote').click(function(){
			var visualizarLote = window.open( '../elabrev/geral/visualizarPagamentoLote.php?codncsiafi='+$(this).attr('codigo'), 'Combopopup', "height=190,width=400,scrollbars=yes,top=250,left=200" );
			visualizarLote.focus();
		});

		// Inserir linha de previsao orcamentaria
		$('.inserirPrev').click(function(e){
            e.preventDefault();
			var tamanho = $('#previsao tr:last').attr('id').replace('tr_','')
              , index = $("[name=crdmesexecucao[]]").length;
			if( tamanho == 'titulo' ){
				tamanho = 0;
			}
			tamanho = parseInt(tamanho)+1;
			tamanho = tamanho+'NEW';
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=novaLinhaPrevisao&cod="+tamanho,
				async: false,
				success: function(msg) {
					$('#previsao tr:last').after(msg);
					$.each($("[name=crdmesexecucao[]]"), function(i, v) {
						if (i==0) {
							valor = $(v).val();
						} else {
							$(v).val(valor).attr('disabled', true);
						}
					});	
				}
			});
		});

        // Inserir linha de previsso orcamentsria quando for usar saldo disponivel a remanejar
        $('.inserirPrevSaldoRemanejado').click(function(e){
            e.preventDefault();
            var tamanho = $('#previsao tr:last').attr('id').replace('tr_','')
                , index = $("[name=crdmesexecucao[]]").length;
            if( tamanho == 'titulo' ){
                tamanho = 0;
            }
            tamanho = parseInt(tamanho)+1;
            tamanho = tamanho+'NEW';
            $.ajax({
                type: "POST",
                url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
                data: "req=novaLinhaPrevisao&cod="+tamanho+"&index="+index,
                async: false,
                success: function(msg){
                    $('#previsao tr:last').after(msg);
                    $.each($(".crdmesexecucao"), function(i, v){
                        if(i==0){
                            valor = $(v).val();
                        }else{
                            $(v).val(valor).attr('disabled', true);
                        }
                    });
                }
            });
        });

		// Botao excluir linha previsao or?amentaria
		$('.excluirPrevisao').live('click',function(){
			if(confirm('Deseja deletar este registro?')){
				var id = $(this).attr('id');				
				if(id.indexOf('NEW')>0){
					$(this).parent().parent().remove();
				}else{
					$.ajax({
						url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao',
						type	: 'post',
						data	: 'requisicao=excluirPrevisao&id='+id,
						success	: function(e){		
							if(e=='1'){
								alert('Operação realizada com sucesso.');
								$('#tr_'+id).remove();
                                location.href="/elabrev/elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao";
							}else{
								alert('Falha ao tentar excluir o anexo!');
							}
						}
					});
				}
			}
		});

		// Seleciona plano de trabalho
		$('[name="ptrid[]"]').live('change',function(){		
			linha = $(this).parent().attr('id');
			linha = linha.split('_');
			var ptrid = $(this).val();

            if (!ptrid) return false;

			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaNomeAcao&ptrid="+ptrid,
				async: false,
				success: function(msg){
					jQuery('#td_acao_'+linha[2]).html(msg);
				}
			});
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaPI&ptrid="+ptrid,
				async: false,
				success: function(msg){
					jQuery('#td_pi_'+linha[2]).html(msg);
				}
			});		
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaDescAcao&ptrid="+ptrid,
				async: false,
				success: function(msg){				
					jQuery('#td_acaodsc_'+linha[2]).html(msg);
				}
			});
			return true;
		});

		var urlPlanoTrabalho = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao&requisicao=carregaPlanosTrabalho';

		$("[name^='ptrid_temp']").autocomplete(urlPlanoTrabalho, {
			matchContains: true,
			minChars: 0,
			cacheLength:1000,
			width: 440,
			autoFill: false,
            max: 1000
		}).result(function(event, data, formatted) {
			
			id_temp = event.target.id
			arTemp = id_temp.split('_');
			$("#ptrid_"+arTemp[1]).val(data[1]);
						
			var ptrid = data[1];

            if (!ptrid) return false;
			
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaNomeAcao&ptrid="+ptrid,
				async: false,
				success: function(msg){									
					jQuery('#td_acao_'+arTemp[1]).html(msg);
				}
			});
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaPI&ptrid="+ptrid,
				async: false,
				success: function(msg){
					jQuery('#td_pi_'+arTemp[1]).html(msg);
				}
			});
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaDescAcao&ptrid="+ptrid,
				async: false,
				success: function(msg){				
					jQuery('#td_acaodsc_'+arTemp[1]).html(msg);
				}
			});
		});

		// Calcula subtotais por ano		
		$("[name^='provalor'], [name^='proanoreferencia']").live('change', function(){

			var arrAnos = new Array();
						
			$("[name^='proanoreferencia']").each(function(i,v){			
				if($.inArray($(v).val(), arrAnos)<0){					
					arrAnos.push($(v).val());					
				}
			});

			var arrAnosTd = new Array();
			$("[id^='td_subtotalano_']").each(function(i,v){
				anoTd = $(v).attr('id').split('_')[2];			
				if($.inArray(anoTd, arrAnosTd)<0){					
					arrAnosTd.push(anoTd);					
				}
			});

			// Oculta/Mostra td com anos sem valores
			if(arrAnosTd.length>0){
				$.each(arrAnosTd, function(indice, ano){
					if($.inArray(ano, arrAnos)<0){
						if($('#td_subtotalano_'+ano).length>0){
							$('#td_subtotalano_'+ano).parent().hide();
						}
					}else{
						if($('#td_subtotalano_'+ano).length>0){
							$('#td_subtotalano_'+ano).parent().show();
						}
					}
				});
			}
			
			// Soma subtotais
			if(arrAnos.length>0){
				$.each(arrAnos, function(indice,ano){					
					subTotalAno = 0;					
					$("[name^='proanoreferencia']").each(function(key,obj){
						if($(obj).val() == ano){
							id = $(obj).attr('id').split('_')[1];

							valor = $('#provalor_'+id).val();
							
							if(valor)
								valor = replaceAll(valor, '.', '').replace(',', '.');
							else
								valor = 0;

							subTotalAno = parseFloat(subTotalAno)+parseFloat(valor);							
						}
					});

					if(subTotalAno>0){
						subTotalAno = MascaraMonetario(''+subTotalAno+'');
					}
					
					if($('#td_subtotalano_'+ano).length == 0){					
						
						var tamanho = $('#previsao tr:last').attr('id').replace('tr_','');
						if( tamanho == 'titulo' ){
							tamanho = 0;
						}
						tamanho = parseInt(tamanho)+1;
						tamanho = tamanho+'NEW';
						
						strNewTd = '<tr bgcolor="#f0f0f0" id="tr_'+tamanho+'">';
						strNewTd +=	'<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';
						
						//strNewTd += '<td align="right"><b>Subtotal ('+ano+')</b>&nbsp;</td>';
						//strNewTd += '<td align="right" style="font-weight:bold;" id="td_subtotalano_'+ano+'">R$ '+subTotalAno+'</td>';
						
						strNewTd += '<td align="right">&nbsp;</td>';
						strNewTd += '<td align="right">&nbsp;</td>';
						
						strNewTd += '<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';							
						strNewTd += '</tr>';
						
						$('#previsao tr:last').after(strNewTd);
						
					}else{
						
						$('#td_subtotalano_'+ano).text('R$ '+subTotalAno+'');
						
					}
					
				});
			}
		});
	});

    function organizaMesExecucao() {
        $("select[name='crdmesexecucao[]']").addClass("crdmesexecucao");
        $.each($("[name=crdmesexecucao[]]"), function(i, v){
            if (i==0) {
                valor = $(v).val();
                $(v).val(valor).attr('disabled', false);
            } else {
                $(v).val(valor).attr('disabled', true);
            }
        });
    }
</script>
<?php

if(!$boPopup) require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo(MODULO_NAME, '' );
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();

?>
<form method="post" name="formulario" id="formulario">

	<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
	
	<input type="hidden" id="abaAtual" name="abaAtual" value="<?=$_REQUEST['aba'] ?>" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom">
				<center><b>Previsão Orçamentária</b></center>
                <?php if (existeSaldoRemanejado($_SESSION['elabrev']['tcpid'])) : ?>
                    <?= pegaSaldoRemanejado($_SESSION['elabrev']['tcpid']); ?>
                <?php endif; ?>
			</td>
		</tr>
	</table>

	<table id="previsao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
		<tr id="tr_titulo">
			<td class="subtitulocentro">&nbsp;</td>
			<td class="subtitulocentro" >&nbsp;</td>
			<td class="subtitulocentro"  width="10%">Ano</td>
			<td class="subtitulocentro" width="">Ação</td>
			<td class="subtitulocentro" width="14%">Programa de Trabalho</td>
			<td class="subtitulocentro" width="10%">Plano Interno</td>
			<td class="subtitulocentro" width="">Descrição da Ação Constante da LOA</td>
			<td class="subtitulocentro" width="20%">Nat.da Despesa</td>
			<td class="subtitulocentro" width="13%">Valor (em R$ 1,00)</td>
			<td class="subtitulocentro" width="8%">Mês da Liberação</td>
			<td class="subtitulocentro" width="20%">Prazo para o cumprimento do objeto</td>
		</tr>
		<?php
        //ver($_SESSION['elabrev']['tcpid']);
		$sql = "
            SELECT DISTINCT
                pro.proid,
                ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as ptrid_descricao,
                substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as pliid_descricao,
                substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as ndp_descricao,
                pro.ptrid,
                a.acacod,
                pro.pliid,
                case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acatitulo,
                pro.ndpid,
                case
                    when (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid) IS NOT NULL then
                        to_char(pro.provalor - (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid), '999G999G999G999G999D99')
                    else
                        to_char(pro.provalor, '999G999G999G999G999D99')
                end as provalor,
                case
                    when (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid) IS NOT NULL then
                        coalesce(pro.provalor - (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid), 0)
                    else
                        coalesce(pro.provalor, 0)
                end as valor,
                crdmesliberacao,
                crdmesexecucao,
                pro.proid,
                pro.proanoreferencia,
                pro.prodata,
                (select ppa2.codncsiafi from elabrev.previsaoparcela ppa2 where ppa2.ppaid = (select max(ppa1.ppaid) from elabrev.previsaoparcela ppa1 where ppa1.proid = pro.proid) and ppa2.ppacancelarnc = 'f') as lote,
                pp.codsigefnc,
                pp.codncsiafi,
                tc.ungcodconcedente
            FROM monitora.previsaoorcamentaria pro
            LEFT JOIN monitora.pi_planointerno pi 		ON pi.pliid = pro.pliid
            LEFT JOIN monitora.pi_planointernoptres pts ON pts.pliid = pi.pliid
            LEFT JOIN public.naturezadespesa ndp 		ON ndp.ndpid = pro.ndpid
            LEFT JOIN monitora.ptres p 					ON p.ptrid = pro.ptrid
            LEFT JOIN monitora.acao a 					ON a.acaid = p.acaid
            LEFT JOIN public.unidadegestora u 			ON u.unicod = p.unicod
            LEFT JOIN monitora.pi_planointernoptres pt 	ON pt.ptrid = p.ptrid
            LEFT JOIN elabrev.previsaoparcela pp		ON (pp.proid = pro.proid)
            LEFT JOIN monitora.termocooperacao tc       ON (tc.tcpid = pro.tcpid)
            LEFT JOIN public.unidadegestora unc         ON (unc.ungcod = tc.ungcodconcedente)
            WHERE pro.prostatus = 'A'
            AND pro.tcpid = ".$_SESSION['elabrev']['tcpid']."
            AND pp.ppanumcancelanc is null
            ORDER BY lote, pro.proanoreferencia DESC, crdmesliberacao
        ";
		//dbg(htmlentities($sql)); die;
		$enviarPagamento = false;
		$dados = !empty($_SESSION['elabrev']['tcpid']) ? $db->carregar($sql) : array();
        //ver($dados);

			if ($dados[0] != '') {

				$arAnosPrevisao = array();
				$arLote = array();
				$totalPrevisao = count($dados)-1;
                //dbg($dados); die;

				foreach ($dados as $k => $dado) {

                    //verifica se o valor total da celula foi remanejado
                    if ((int)$dado['provalor'] <= 0) {
                        if ($db->pegaLinha("select * from elabrev.creditoremanejado where proid = {$dado['proid']}"))
                            continue;
                    }

					#Imprime subtotal por lote

					if (!in_array($dado['lote'], $arLote)) {
		  				if ($subTotalPorLote>0) {
                            echo '
                                <tr bgcolor="#f0f0f0" id="tr_'.$k.'">
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                    <td align="right"><img align="absmiddle" src="../imagens/icone_lupa.png" border="0" class="linkLote" codigo="'.$loteAnterior.'" alt="Visualizar lote" title="Visualizar lote"/>&nbsp;<span class="linkLote" codigo="'.$loteAnterior.'"><b>Nota de crédito ('.($loteAnterior ? $loteAnterior : 'lote n?o encontrado').')</b></span>&nbsp;</td>
                                    <td align="right" id="td_subtotalano_'.($loteAnterior ? $loteAnterior : '0000').'" style="font-weight:bold;">R$ '.formata_valor($subTotalPorLote).'</td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                            ';
		  				}
						array_push($arLote, $dado['lote']);
	  					$subTotalPorLote = 0;
	  					$loteAnterior = $dado['lote'];
					}
				?>
				<tr id="tr_<?=$dado['proid']?>">

					<td align="center">
                        <?php
                        //echo $dado['proid'];
                        if ($estadoAtual == EM_EXECUCAO) {

                            if (empty($dado['codncsiafi']) && $dado['ungcodconcedente'] != UG_FNDE) { // Termos Nao FNDE
                                $enviarPagamento = true;
                                echo "<input name='checkEnvio' id='checkEnvio' value='{$dado['proid']}' type='checkbox'/>";
                            } else if (empty($dado['codsigefnc']) && $dado['ungcodconcedente'] == UG_FNDE) { //Termos do FNDE
                                $enviarPagamento = true;
                                echo "<input name='checkEnvio' id='checkEnvio' value='{$dado['proid']}' type='checkbox'/>";
                            }

                        } else {
                            if ( $concedente != 'fnde' && !$dado['lote'] && in_array($estadoAtual, $arEstadosDocCadNC) &&
                                (in_array(PERFIL_CGSO, $perfis) || in_array(CGO_EQUIPE_ORCAMENTARIA, $perfis) || in_array(UO_COORDENADOR_EQUIPE_TECNICA, $perfis) || $db->testa_superuser())){
                                $enviarPagamento = true;
                                echo "<input name='checkEnvio' id='checkEnvio' value='{$dado['proid']}' type='checkbox'/>";
                            }
                        }


                        //verifica se existe historico de termo em diligencia
                        $hstDiligencia = $db->pegaUm("select count(*) from workflow.historicodocumento hst where hst.aedid = 1607 and hst.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})");
                        //verifica se existe historico de termo em execucao
                        $hstExecucao = $db->pegaUm("select count(*) from workflow.historicodocumento hst where hst.aedid in (1609, 1618, 1650, 2440) and hst.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})");

                        //verifica se ja foi solicitado NC para previsao orçamentaria
                        $creditoLancado = $db->pegaUm("select * from elabrev.previsaoparcela where proid = {$dado['proid']}");

                        if (($estadoAtual == ALTERAR_TERMO_COOPERACAO && verificaEquipeTecnicaProponente() && $creditoLancado)
                            || ($estadoAtual == EM_DILIGENCIA && $hstDiligencia && $hstExecucao && $creditoLancado)) {
                            echo '<img border="0" id="'.$dado['proid'].'" class="remanejarNc" title="Remanejar Recurso" style="cursor: pointer" src="/imagens/startbar_refresh_over.bmp">';
                        }

                        ?>
					</td>

					<td align="center">
						<?php if ($habilita_botao == 'S') { ?>
							<?php if ($k <> '0' && empty($dado['codsigefnc']) && empty($dado['codncsiafi'])) { ?>
                                <img border="0" id="<?=$dado['proid'] ?>" class="excluirPrevisao" title="Excluir" style="cursor: pointer" src="/imagens/excluir.gif">
							<?php } ?>
						<?php } else { ?>
							<img border="0" id="<?=$k ?>" title="Excluir" src="/imagens/excluir_01.gif">
						<?php } ?>
						<input type="hidden" name="linha[]" value="<?=$dado['proid'] ?>"/>
					</td>

					<td align="center" id="td_anoref_<?=$dado['proid']; //$k ?>" width="14%">
						<?php if($habilita_Natur == 'S'){?>
							<?php
							for($z=0;$z<=10;$z++){
								$arAnosRef[$z]['codigo']	= 2013+$z;
								$arAnosRef[$z]['descricao']	= 2013+$z;
							}

							$db->monta_combo('proanoreferencia[]',$arAnosRef, 'S', 'Selecione...','',$opc,'','','S', 'proanoreferencia_'.$dado['proid'], '', $dado['proanoreferencia'], $title= null);
							?>
						<?php }else{ ?>
							<?php echo $dado['proanoreferencia'] ? $dado['proanoreferencia'] : '-'; ?>
							<input type="hidden" name="proanoreferencia[]" id="proanoreferencia_<?php echo $dado['proid']; ?>" value="<?=$dado['proanoreferencia'] ?>"/>
						<?php } ?>
					</td>

					<td align="center" id="td_acao_<?=$k ?>">
						<?=$dado['acacod'] ?>
					</td>

					<?php if ($habilita_Plano == 'S') { ?>
						<td align="center" id="td_prg_<?=$dado['proid']; //$k ?>"  width="10%" >
							<?php
							$sql = "SELECT DISTINCT
										p.ptrid as codigo,
		 								ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
									FROM monitora.ptres p
									JOIN public.unidadegestora u 
										ON u.unicod = p.unicod
									WHERE p.ptrano = '2013' 
									AND p.ptrstatus = 'A'
									AND u.unicod IN ( '26101','26298','26291','26290' )";

							//$db->monta_combo('ptrid[]',$sql, 'S', 'Selecione...','',$opc,'','200','S', 'combobox', '', $dado['ptrid'], $title= null);
							?>
							<?=campo_texto('ptrid_temp[]','S','S','',20,27,'','', 'left', '', '', 'id="ptridtemp_'.$dado['proid'].'"', '', $dado['ptrid_descricao']);?>
							<input type="hidden" name="ptrid[]" id="ptrid_<?php echo $dado['proid']; ?>" value="<?php echo $dado['ptrid']; ?>" />
						</td>
					<?php } else { ?>
						<td align="center" id="td_prg_<?=$k ?>">
							<input type="hidden" name="ptrid[]" value="<?=$dado['ptrid']?>">
							<?=$dado['ptrid_descricao']?$dado['ptrid_descricao']:'-'?>
						</td>
					<?php } ?>

					<?php if ($habilita_Plano == 'S') { ?>
						<td align="center" id="td_pi_<?=$dado['proid']; //$k ?>"  width="10%">
							<?php
								if ($dado['ptrid'] != '') {
									$sql = "
										SELECT p.pliid as codigo,
												plicod||' - '||plidsc as descricao
										FROM monitora.pi_planointerno p
										INNER JOIN monitora.pi_planointernoptres pt on pt.pliid = p.pliid
										WHERE pt.ptrid = ".$dado['ptrid']." 
										ORDER by 2
									";

									echo $db->monta_combo('pliid[]',$sql, 'S', 'Selecione...','',$opc,'','85','S', 'pliid', '', $dado['pliid'], $title= null);
								}
							?>
						</td>
					<?php } else { ?>
						<td align="center" id="td_pi_<?=$dado['proid']; //$k ?>">
							<input type="hidden" name="pliid[]" value="<?=$dado['pliid']?>">
							<?=$dado['pliid_descricao']?$dado['pliid_descricao']:'-'?>
						</td>
					<?php } ?>

					<td align="center" id="td_acaodsc_<?=$dado['proid']; //$k ?>">
						<?=$dado['acatitulo']?$dado['acatitulo']:'-'?>
					</td>

					<?php if ($habilita_Natur == 'S') { ?>
						<td align="center" width="23%">
							<?php
								$sql = "
									SELECT 	DISTINCT ndpid as codigo,
											substr(ndpcod, 1, 6) || ' - ' || ndpdsc as descricao
									FROM public.naturezadespesa
									WHERE ndpstatus = 'A' and sbecod = '00' and edpcod != '00' and substr(ndpcod,1,2) not in ( '31', '32', '46', '34' )
									AND ( substr(ndpcod, 3, 2) in ('80', '90', '91','40') or substr(ndpcod, 1, 6) in ('335041','339147','335039', '445041', '333041') )
									order by 2
								";
								$db->monta_combo('ndpid[]',$sql,'S','Selecione...','',$opc,'','250','S', 'ndpid', '', $dado['ndpid'], $title= null);
							?>
						</td>
					<?php } else {?>
						<td align="center">
							<input type="hidden" name="ndpid[]" value="<?=$dado['ndpid']?>">
							<?=$dado['ndp_descricao']?$dado['ndp_descricao']:'-'?>
						</td>
					<?php } ?>

					<?php if ($habilita_Natur == 'S') { ?>
						<td align="center" width="16%" >
							<?=campo_texto('provalor[]','S','S','',17,17,'[.###],##','', 'right', '', '', 'id="provalor_'.$dado['proid'].'"', '', $dado['provalor']);?>
						</td>
					<?php } else { ?>
						<td align="center">
							<input type="hidden" name="provalor[]" value="<?=$dado['provalor']?>">
							<?=$dado['provalor']?>
						</td>
					<?php } ?>

					<?php if($habilita_Meslib == 'S'){ ?>
						<td align="center" width="10%">
							<?php
								$sql = Array(Array('codigo'=>1,'descricao'=>'Janeiro'),
											 Array('codigo'=>2,'descricao'=>'Fevereiro'),
											 Array('codigo'=>3,'descricao'=>'Março'),
											 Array('codigo'=>4,'descricao'=>'Abril'),
											 Array('codigo'=>5,'descricao'=>'Maio'),
											 Array('codigo'=>6,'descricao'=>'Junho'),
											 Array('codigo'=>7,'descricao'=>'Julho'),
											 Array('codigo'=>8,'descricao'=>'Agosto'),
											 Array('codigo'=>9,'descricao'=>'Setembro'),
											 Array('codigo'=>10,'descricao'=>'Outubro'),
											 Array('codigo'=>11,'descricao'=>'Novembro'),
											 Array('codigo'=>12,'descricao'=>'Dezembro')
										);
								$db->monta_combo('crdmesliberacao[]',$sql,'S','Selecione...','',$opc,'','85','S', 'crdmesliberacao', '', $dado['crdmesliberacao'], $title= null);
							?>
						</td>
					<?php }else{ ?>
						<td align="center"  width="50%">
							<input type="hidden" name="crdmesliberacao[]" id="'crdmesliberacao[]" value="<?=$dado['crdmesliberacao']?>">
							<?php echo $dado['crdmesliberacao'] ? mes_extenso($dado['crdmesliberacao']) : '-';?>
						</td>
					<?php }?>

					<?php if ($habilita_Parc == 'S') { ?>
						<td align="center"  width="50%">
							<?php
								$sql = array();
								for($i = 1; $i <= 50; $i++){
									$sql[$i-1]['codigo'] = $i;
									$sql[$i-1]['descricao'] = $i.' Mês(s)';
								}
								array_push($sql, $sql);
                                $txtDica = 'Favor, preencher o novo prazo total de execução do TED, contado da data de início da vigência até o prazo final, incluindo o termo aditivo.';
								$db->monta_combo('crdmesexecucao[]', $sql,'S','Selecione...','', $opc, $txtDica, '100','S', 'crdmesexecucao', '', $dado['crdmesexecucao'], $title= null);
							?>
						</td>
					<?php }  else { ?>
						<td align="center">
							<input type="hidden" name="crdmesexecucao[]" id="'crdmesexecucao[]" value="<?=$dado['crdmesexecucao']?>">
							<?php echo $dado['crdmesexecucao'].' Mês(s)' ?>
						</td>
					<?php }?>
				</tr>
				<?php

				// TODO: realizar a somatorio baseada no mes
					if( $dado['lote'] != null )
						$subTotalPorLote = $subTotalPorLote+$dado['valor'];

					#Imprime ultimo subtotal se houver
					if($totalPrevisao==$k && $loteAnterior){
						echo '
						<tr bgcolor="#f0f0f0" id="tr_'.($k+1).'">
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td align="right"><span class="linkLote" codigo="'.$loteAnterior.'"><b>Nota de crédito ('.$loteAnterior.')</b></span>&nbsp;</td>
							<td align="right" id="td_subtotalano_'.($loteAnterior ? $loteAnterior : '0000').'" style="font-weight:bold;">R$ '.formata_valor($subTotalPorLote).'</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>							
						</tr>
						';
					}
				}
			}
		?>
	</table>

    <?php

    $strSQL = "
        select count(*) as total from elabrev.creditoremanejado where proid in (
            select proid from monitora.previsaoorcamentaria
            where tcpid = {$_SESSION['elabrev']['tcpid']} and statusvalorzerado = 't'
        )
    ";

    if ($db->pegaUm($strSQL)) {
        include APPRAIZ . 'elabrev/modulos/principal/termoCooperacao/previsao/credito-zerado.inc';
    }

    ?>

    <?php
    if ((in_array($estadoAtual, array(EM_CADASTRAMENTO, EM_DILIGENCIA, ALTERAR_TERMO_COOPERACAO))) || in_array(PERFIL_CGSO, $perfis)) {
        $habilitaInserir = 'S';
    }
    ?>

    <?php

    /**
     * Pedido do Guilherme em 28/08/2014
     * Quando esta em solicitacao de alteracao, somente equipe tecnica proponetne
     * pode fazer aditivo no termo
     */
    if ($estadoAtual == ALTERAR_TERMO_COOPERACAO && verificaEquipeTecnicaConcedente() && !in_array(PERFIL_SUPER_USUARIO, $perfis)) {
        $habilitaSalvar = 'N';
        $habilitaInserir = 'N';
    }
    ?>

	<?php if (($habilitaInserir == 'S' && in_array($estadoAtual, array(EM_CADASTRAMENTO, EM_DILIGENCIA, ALTERAR_TERMO_COOPERACAO)))
          || (in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_CGSO, $perfis))): ?>

		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
			<tr>
                <?php if (!existeSaldoRemanejado($_SESSION['elabrev']['tcpid'])) : ?>
                    <td colspan="2">
                        <a href="#" class="inserirPrev">
                            <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Contatos">
                            Inserir Nova Previsão
                        </a>
                    </td>
                <?php endif; ?>

                <?php if($concedente != 'fnde' && in_array($estadoAtual, $arEstadosDocCadNC) && $enviarPagamento): ?>
                    <td style="width:56%;">
                        <input type='button' onclick='javascript:janelaDeEnvioParaPagamento();' value='Cadastrar NC'/>
                    </td>
                <?php endif; ?>

                <?php //if (in_array($estadoAtual, array(EM_CADASTRAMENTO)) && in_array(UO_EQUIPE_TECNICA, $perfis) && !existeSaldoRemanejado($_SESSION['elabrev']['tcpid'])) : ?>
                    <td>
                        <a href="#" class="inserirPrev">
                            <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Contatos">
                            Inserir Nova Previsão
                        </a>
                    </td>
                <?php //endif; ?>
			</tr>
		</table>
	<?php else :

        if (($habilitaInserir == 'S' && in_array($estadoAtual, array(EM_CADASTRAMENTO, EM_ANALISE_OU_PENDENTE)) && !existeSaldoRemanejado($_SESSION['elabrev']['tcpid']))
            && (in_array(UO_EQUIPE_TECNICA, $perfis) || in_array(PERFIL_COORDENADOR_SEC, $perfis))) : ?>
            <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
                <tr>
                    <td colspan="2">
                        <a href="#" class="inserirPrev">
                            <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Contatos">
                            Inserir Nova Previsão
                        </a>
                    </td>
                </tr>
            </table>
        <?php endif; ?>

    <?php endif; ?>

    <?php if ($habilitaInserir == 'S' && $estadoAtual == ALTERAR_TERMO_COOPERACAO && existeSaldoRemanejado($_SESSION['elabrev']['tcpid'])) : ?>
        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
            <tr>
                <td colspan="2">
                    <a href="#" class="inserirPrevSaldoRemanejado">
                        <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Usar saldo disponível">
                        Usar saldo a remanejar
                    </a>
                </td>
            </tr>
        </table>
    <?php endif; ?>

    <?php if ($estadoAtual == EM_DILIGENCIA && existeSaldoRemanejado($_SESSION['elabrev']['tcpid']) && $hstDiligencia && $hstExecucao) : ?>
        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
            <tr>
                <td colspan="2">
                    <a href="#" class="inserirPrevSaldoRemanejado">
                        <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Usar saldo disponível">
                        Usar saldo a remanejar
                    </a>
                </td>
            </tr>
        </table>
    <?php endif; ?>

    <?php
    if ($estadoAtual == EM_ANALISE_OU_PENDENTE && verificaEquipeTecnicaConcedente()) {
        $habilitaSalvar = 'S';
        $habilitaInserir = 'S';
    }
    ?>

	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td colspan="3" align="center">
				<?php if($habilitaSalvar == 'S'): ?>
					<input type="button" id="descentralizacao" value="Gravar e Voltar" class="navegarSalvar"/> 
					<input type="button" id="salva" value="Gravar" class="salvar"/>
					<input type="button" id="<?php echo in_array(UO_EQUIPE_TECNICA, $perfis) ? 'anexo' : 'parecertecnico' ?>" value="Gravar e Continuar" class="navegarSalvar"/>
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>

<div class="tb_modal"></div>

<script type="text/javascript">
$(function(){
    $(".remanejarNc").click(function(e){
        e.preventDefault();

        var _proid = $(this).attr('id');
        if (!_proid) return false;

        $.ajax({
            url:"/elabrev/elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao"
            , type:"POST"
            , data:{remanejar_id:_proid}
            , beforeSend: function(){
                overLay();
            }
            , success: function(data){
                $(".tb_modal").css({
                    "width":"1350px",
                    "height":"420px",
                    "top": "50%",
                    "left": "50%",
                    "margin-left":"-675px",
                    "margin-top": "-210px",
                    "border": "1px solid #000",
                    "display": "block",
                    "position": "fixed",
                    "background": "#fff",
                    "overflow": "auto",
                    "z-index": 100000000
                }).html(data);
            }
            , complete: function(){
                modalClose();
            }
        });
    });
});

function overLay() {
    var backgroundElement = jQuery("<div id=\"overlay\" ></div>");
    backgroundElement.appendTo("body");
    backgroundElement.css({
        'width': jQuery(document).width(),
        'height': jQuery(document).height(),
        'display': 'none',
        'background-color': '#555555',
        'position': 'absolute',
        'top': 0,
        'left': 0,
        'z-index': 9990
    });
    backgroundElement.fadeTo('fast', 0.8);
}

function modalClose() {
    jQuery(".fecharBtn").click(function(e){
        e.preventDefault();
        jQuery(".tb_modal").html("").css("display", "none");
        jQuery("#overlay").remove();
    });
}

function janelaDeEnvioParaPagamento(){

	var celulasOrcamentarias = new Array();
	$.each( $('[name="checkEnvio"]'), function(index,value){ 
		if( $(value).is(':checked') == true ){
			celulasOrcamentarias.push( $(value).parent().parent().attr('id').substring(3) );
		}
	});
	if( celulasOrcamentarias.length < 1 ){
		alert('Deve haver pelo menos 1 célula or?amentária marcada para envio.');
		return false;
	}

	var pagamento = window.open( '../elabrev/geral/envioParaPagamento.php', 'Combopopup', "height=180,width=400,scrollbars=yes,top=250,left=200" );
	pagamento.focus();
}


$(function(){
    // Bloqueia mes de execucao
    organizaMesExecucao();
});

/**
 * controla a opcao de alterar o prazo do objeto
 * mantendo todas as opcoes da previsao orcamentaria desabilitadas
 * somente a primeira faz o controle total
 */
$('.crdmesexecucao').live('change',function(){
    valor = this.value;
    $.each($('.crdmesexecucao'), function(i,v){
        if (i>0) {
            $(v).attr('disabled', false).val(valor).attr('disabled', true);
        }
    });
});
</script>

<?php

if (!teste_superUser()) {
    // trava o formulario para os termos que ja foram enviados solicitados Nota de crédito
    $sqlTrava = "
        select proid from elabrev.previsaoparcela where proid IN (
          select proid from monitora.previsaoorcamentaria where tcpid = {$_SESSION['elabrev']['tcpid']} and prostatus = 'A'
        )
    ";

    $totalPrevisao = $db->pegaUm("
        select count(*) as total from monitora.previsaoorcamentaria
        where tcpid = {$_SESSION['elabrev']['tcpid']} and prostatus = 'A'
    ");

    $proids = $db->carregar($sqlTrava);
    if ($proids) {
        echo '<script type="text/javascript">';
        foreach ($proids as $proid) {
            echo '$("#tr_'.$proid['proid'].' input, #tr_'.$proid['proid'].' select").attr("disabled", true);';
        }

        if ($totalPrevisao == 1){
            echo '$($("select[name=\'crdmesexecucao[]\']")[0]).attr("disabled", false);';
            echo '$($("input[name=\'linha[]\']")[0]).attr("disabled", false);';
            echo '$($("input[name=\'ptrid[]\']")[0]).attr("disabled", false);';
        }
        echo '</script>';
    }
}

/**
 * Trava as celulas orçamentárias caso o estado atual seja "Termo em solicitação de alteração"

$sqlTravaPrevisoes = $db->carregar("select proid from monitora.previsaoorcamentaria where tcpid = {$_SESSION['elabrev']['tcpid']} and prostatus = 'A'");
if (is_array($sqlTravaPrevisoes) && $estadoAtual == ALTERAR_TERMO_COOPERACAO && !teste_superUser()) {
    if ($sqlTravaPrevisoes) {
        echo '<script type="text/javascript">';
        foreach ($sqlTravaPrevisoes as $proid) {
            echo '$("#tr_'.$proid['proid'].' input, #tr_'.$proid['proid'].' select").attr("disabled", true);';
        }
            echo '$($("select[name=\'crdmesexecucao[]\']")[0]).attr("disabled", false);';
            echo '$($("input[name=\'linha[]\']")[0]).attr("disabled", false);';
            echo '$($("input[name=\'ptrid[]\']")[0]).attr("disabled", false);';
        echo '</script>';
    }
}
*/
?>