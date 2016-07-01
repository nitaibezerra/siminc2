<?php 
/*
$servidor_bd_siafi 	= '';
$porta_bd_siafi 	= '';
$nome_bd_siafi 		= '';
$usuario_db_siafi 	= '';
$senha_bd_siafi 	= '';

$servidor_bd = $servidor_bd_siafi;
$porta_bd    = $porta_bd_siafi;
$nome_bd     = $nome_bd_siafi;
$usuario_db  = $usuario_db_siafi;
$senha_bd    = $senha_bd_siafi;

// Cria o novo objeto de conexao
$db2 = new cls_banco();
*/
/*
$sql = "(
		select --distinct  
                c.it_co_credor, c.it_no_credor, 
                ob.numerodaob as ob, 
                to_char(ob.datadetransacao, 'YYYY/MM/DD') || ' ' || to_char(ob.horatransacao, 'HH24:MI') as datatransacao, 
                ob.observacao as obsob, 
                u.ungcod || ' - ' || u.ungdsc as unidade, 
                substr(ob.classificacao_a_1::character varying(9),2,1) || '.' || substr(ob.classificacao_a_1::character varying(9),3,1) || '.' || substr(ob.classificacao_a_1::character varying(9),4,2) 
                || '.' || substr(ob.classificacao_a_1::character varying(9),6,2) || '.' || substr(ob.classificacao_a_1::character varying(9),8,2) as natureza, 
                c.it_co_credor ||' - '|| c.it_no_credor as favorecido, 
                ob.valordatransacao_1 as valorob 
        from siafi2012.ob ob 
        inner join ( select c.it_co_credor, c2.it_no_credor 
                     from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao 
                     from siafi2012.credor c 
                     where length(c.it_co_credor) = 14 and c.it_co_tipo_crdor = '1' 
                     group by c.it_co_credor) c 
          			 inner join siafi2012.credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao ) c ON c.it_co_credor = ob.codigodofavorecido 
          inner join dw.ug u ON u.ungcod::integer = ob.codigodaugdooperador 
          where 
          ob.codigodaugdooperador = '{$_GET['ungcod']}' and
		  c.it_co_credor = '{$_GET['cnpj']}' and trim(codigodainscricao_a_1)='{$_GET['empenho']}'
		) 
		UNION ALL
		(
		select --distinct  
                c.it_co_credor, c.it_no_credor, 
                ob.numerodaob as ob, 
                to_char(ob.datadetransacao, 'YYYY/MM/DD') || ' ' || to_char(ob.horatransacao, 'HH24:MI') as datatransacao, 
                ob.observacao as obsob, 
                u.ungcod || ' - ' || u.ungdsc as unidade, 
                substr(ob.classificacao_a_1::character varying(9),2,1) || '.' || substr(ob.classificacao_a_1::character varying(9),3,1) || '.' || substr(ob.classificacao_a_1::character varying(9),4,2) 
                || '.' || substr(ob.classificacao_a_1::character varying(9),6,2) || '.' || substr(ob.classificacao_a_1::character varying(9),8,2) as natureza, 
                c.it_co_credor ||' - '|| c.it_no_credor as favorecido, 
                ob.valordatransacao_1 as valorob 
        from siafi2013.ob ob 
        inner join ( select c.it_co_credor, c2.it_no_credor 
                     from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao 
                     from siafi2013.credor c 
                     where length(c.it_co_credor) = 14 and c.it_co_tipo_crdor = '1' 
                     group by c.it_co_credor) c 
          			 inner join siafi2013.credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao ) c ON c.it_co_credor = ob.codigodofavorecido 
          inner join dw.ug u ON u.ungcod::integer = ob.codigodaugdooperador 
          where 
          ob.codigodaugdooperador = '{$_GET['ungcod']}' and
		  c.it_co_credor = '{$_GET['cnpj']}' and trim(codigodainscricao_a_1)='{$_GET['empenho']}'
		) 
		UNION ALL
		(
		select --distinct  
                c.it_co_credor, c.it_no_credor, 
                ob.numerodaob as ob, 
                to_char(ob.datadetransacao, 'YYYY/MM/DD') || ' ' || to_char(ob.horatransacao, 'HH24:MI') as datatransacao, 
                ob.observacao as obsob, 
                u.ungcod || ' - ' || u.ungdsc as unidade, 
                substr(ob.classificacao_a_1::character varying(9),2,1) || '.' || substr(ob.classificacao_a_1::character varying(9),3,1) || '.' || substr(ob.classificacao_a_1::character varying(9),4,2) 
                || '.' || substr(ob.classificacao_a_1::character varying(9),6,2) || '.' || substr(ob.classificacao_a_1::character varying(9),8,2) as natureza, 
                c.it_co_credor ||' - '|| c.it_no_credor as favorecido, 
                ob.valordatransacao_1 as valorob 
        from siafi2014.ob ob 
        inner join ( select c.it_co_credor, c2.it_no_credor 
                     from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao 
                     from siafi2014.credor c 
                     where length(c.it_co_credor) = 14 and c.it_co_tipo_crdor = '1' 
                     group by c.it_co_credor) c 
          			 inner join siafi2014.credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao ) c ON c.it_co_credor = ob.codigodofavorecido 
          inner join dw.ug u ON u.ungcod::integer = ob.codigodaugdooperador 
          where 
          ob.codigodaugdooperador = '{$_GET['ungcod']}' and
		  c.it_co_credor = '{$_GET['cnpj']}' and trim(codigodainscricao_a_1)='{$_GET['empenho']}'
		)";
*/

/*
$sql = "(
         select --distinct 
                c.it_co_credor, c.it_no_credor,
                ob.numero_ob as ob,
                to_char(ob.data_transacao, 'YYYY/MM/DD') || ' ' || to_char(ob.hora_transacao, 'HH24:MI') as datatransacao,
                ob.observacao as obsob,
                u.ungcod || ' - ' || u.ungdsc as unidade,
                substr(ob.classificacao1_01::character varying(9),2,1) || '.' || substr(ob.classificacao1_01::character varying(9),3,1) || '.' || substr(ob.classificacao1_01::character varying(9),4,2)
                || '.' || substr(ob.classificacao1_01::character varying(9),6,2) || '.' || substr(ob.classificacao1_01::character varying(9),8,2) as natureza,
                c.it_co_credor ||' - '|| c.it_no_credor as favorecido,
                ob.valor_transacao_01::text as valorob
        from siafi2012.ob ob
        inner join ( select c.it_co_credor, c2.it_no_credor
                     from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao
                     from siafi2012.credor c
                     where length(c.it_co_credor) = 14 and c.it_co_tipo_crdor = '1'
                     group by c.it_co_credor) c
                                                inner join siafi2012.credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao ) c ON c.it_co_credor = ob.codigo_favorecido
          inner join dw.ug u ON u.ungcod = ob.codigo_ug_operador
          where
     
		 --select * from siafi2014.ob limit 10   
          ob.codigo_ug_operador = '{$_GET['ungcod']}' and
          c.it_co_credor = '{$_GET['cnpj']}' and trim(codigo_inscricao1_01)='{$_GET['empenho']}'
                               )
                               UNION ALL
                               (
                               select --distinct 
                c.it_co_credor, c.it_no_credor,
                ob.numero_ob as ob,
                to_char(ob.data_transacao, 'YYYY/MM/DD') || ' ' || to_char(ob.hora_transacao, 'HH24:MI') as datatransacao,
                ob.observacao as obsob,
                u.ungcod || ' - ' || u.ungdsc as unidade,
                substr(ob.classificacao1_01::character varying(9),2,1) || '.' || substr(ob.classificacao1_01::character varying(9),3,1) || '.' || substr(ob.classificacao1_01::character varying(9),4,2)
                || '.' || substr(ob.classificacao1_01::character varying(9),6,2) || '.' || substr(ob.classificacao1_01::character varying(9),8,2) as natureza,
                c.it_co_credor ||' - '|| c.it_no_credor as favorecido,
                ob.valor_transacao_01::text as valorob
                --ob.valor_transacao_01 as valorob
        from siafi2013.ob ob
        inner join ( select c.it_co_credor, c2.it_no_credor
                     from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao
                     from siafi2013.credor c
                     where length(c.it_co_credor) = 14 and c.it_co_tipo_crdor = '1'
                     group by c.it_co_credor) c
                                                inner join siafi2013.credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao ) c ON c.it_co_credor = ob.codigo_favorecido
          inner join dw.ug u ON u.ungcod = ob.codigo_ug_operador
          where
          ob.codigo_ug_operador = '{$_GET['ungcod']}' and
          c.it_co_credor = '{$_GET['cnpj']}' and trim(codigo_inscricao1_01)='{$_GET['empenho']}'
                               )
                               UNION ALL
                               (
        select --distinct 
                c.it_co_credor, c.it_no_credor,
                ob.numerodaob as ob,
                to_char(ob.datadetransacao, 'YYYY/MM/DD') || ' ' || to_char(ob.horatransacao, 'HH24:MI') as datatransacao,
                ob.observacao as obsob,
                u.ungcod || ' - ' || u.ungdsc as unidade,
                substr(ob.classificacao_a_1::character varying(9),2,1) || '.' || substr(ob.classificacao_a_1::character varying(9),3,1) || '.' || substr(ob.classificacao_a_1::character varying(9),4,2)
                || '.' || substr(ob.classificacao_a_1::character varying(9),6,2) || '.' || substr(ob.classificacao_a_1::character varying(9),8,2) as natureza,
                c.it_co_credor ||' - '|| c.it_no_credor as favorecido,
                ob.valordatransacao_1::text as valorob
                --regexp_replace(ob.valordatransacao_1::text, '[R$,]', '', 'g')::numeric as valorob
        from siafi2014.ob ob
        inner join ( select c.it_co_credor, c2.it_no_credor
                     from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao
                     from siafi2014.credor c
                     where length(c.it_co_credor) = 14 and c.it_co_tipo_crdor = '1'
                     group by c.it_co_credor) c
                                                inner join siafi2014.credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao ) c ON c.it_co_credor = ob.codigodofavorecido
          inner join dw.ug u ON u.ungcod::integer = ob.codigodaugdooperador
          where
          ob.codigodaugdooperador = '{$_GET['ungcod']}' and
          c.it_co_credor = '{$_GET['cnpj']}' and trim(codigodainscricao_a_1)='{$_GET['empenho']}'
         )";
         
         $rs = $db2->carregar($sql);
         */
$hspid = $_GET['hspid'];
if( $hspid ){
	$sql = "SELECT
				ungcod
			FROM
				contratos.hospitalug
			WHERE
				hspid = {$hspid}";
	$arUngcod = $db->carregarColuna($sql);
	$arUngcod = (array) $arUngcod;
		
	$whereUnidade = " unidade IN ('" . implode("', '", $arUngcod) . "') ";
}else{
	$whereUnidade = " unidade IN ('') ";
}


$sql = "SELECT obs.it_co_credor, obs.it_no_credor, ob, eps.nu_empenho, eps.epsid,
			   TO_CHAR(obs.datatransacao, 'DD/MM/YYYY') AS datatransacao,
			   TO_CHAR(obs.datatransacao, 'YYYY/MM/DD') AS datatransacao_banco,
--			   TO_CHAR(obs.datatransacao, 'DD/MM/YYYY') || ' ' || TO_CHAR(obs.datatransacao, 'HH24:MI') AS datatransacao,
			   obsob,
			   unidade,
			   obs.natureza,
			   it_co_credor ||' - '|| it_no_credor as favorecido,
			   obs.valor as valorob
  		FROM 
  			contratos.ob_siafi obs
  		JOIN contratos.empenho_siafi eps ON TRIM(eps.nu_empenho) = TRIM(obs.empenho)
  		WHERE 
  				{$whereUnidade} 
		  		AND TRIM(it_co_credor) = TRIM('{$_GET['cnpj']}') 
		  		AND eps.epsid IN (
		  		                  SELECT  
		  		                  	e.epsid 
		  		                  FROM contratos.empenhovinculocontrato e
		  		                  INNER JOIN contratos.empenho_siafi s ON s.epsid = e.epsid
						       	  WHERE 
						       		e.ctrid = {$_SESSION['ctrid']} AND 
						       		co_favorecido = TRIM('{$_GET['cnpj']}') 
		  		                 ) 
				AND obs.ob NOT IN (
									SELECT 
										obf.obfnumero 
									FROM 
										contratos.faturacontrato fc
									JOIN contratos.ordembancariafatura obf ON obf.ftcid = fc.ftcid
									WHERE 
										fc.ftcstatus = 'A' AND
										" . ($_GET['ftcid'] ? " fc.ftcid != {$_GET['ftcid']} AND " : "") . "
										fc.ctrid = {$_SESSION['ctrid']}
									)
		ORDER BY
			obs.datatransacao";
$rs = $db->carregar($sql);
// dbg($sql, d);
$rsObCheck = array();
if($_GET['ftcid']){
	$sql = "select trim(obfnumero) as obfnumero from contratos.ordembancariafatura where ftcid = {$_GET['ftcid']}";
	$rsObCheck = $db->carregarColuna($sql);
}
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
	$(document).ready(function(){	
		$('.checkob').click(function(){

			//alert($(this).val());
			arValor = $(this).val().split('_');
			
			var numob 		  = trim(arValor[0]);
			var vlrob 		  = arValor[1].replace('R$','');
			var dttransacaoob = arValor[2];
			var epsid 	  	  = arValor[3];
			var empenho 	  = arValor[4];
			//alert(numob);
			//alert(vlrob);
			
			if(vlrob) vlrob = mascaraglobal('###.###.###.###,##', vlrob);
			
			if(this.checked){
				
				var tr_clone = $('#tb_numob > tbody > tr:first', window.opener.document).clone();

				var bt_remove = "<img src='../imagens/excluir.gif' onclick=\"$(this).parent().parent().remove(); verificaTotalOb();\" class='link' />";
				
				var nu_ob = numob+'<input type="hidden" name="obfnumero[]" value="'+numob+'" />';

				var dt_ob = '<input type="hidden" name="obfdatatransacao[]" value="'+dttransacaoob+'" />';
				
				var vl_ob = vlrob+'<input type="hidden" name="obfvalor[]" value="'+vlrob+'" onchange="verificaTotalOb()" />';
					vl_ob += '<input type="hidden" name="obfsiafi[]" value="true" />';
					
				var empenho  = empenho+'<input type="hidden" name="epsid[]" value="'+epsid+'" />';
				
				tr_clone.attr('id', numob +'_'+ epsid);
				tr_clone.attr('class', 'obsiafi');				
				tr_clone.children().slice(0,1).html(bt_remove);
				tr_clone.children().slice(1,2).html(nu_ob + dt_ob);
				tr_clone.children().slice(2,3).html(vl_ob);
				tr_clone.children().slice(3,4).html(empenho);
				
				tr_clone.insertAfter($('#tb_numob > tbody > tr:last', window.opener.document));
					
			}else{
				
				$('#'+numob +'_'+ epsid, window.opener.document).remove();
			}

			verificaTotalObPopup();
			
		});
	});

	function verificaTotalObPopup()
	{
		var obCalc = new Calculo();
		
//		var total_valor = 0;
		var total_valor = '0,00';
		var total = $("#ftcvalor", window.opener.document).val();

//		if(total) total = parseFloat(converteStringEmNumero(total));
		
		$.each( $("[name='obfvalor[]']", window.opener.document), function( chave, campo ) {
			
			var valor = $(campo).val();
			
			if(valor){
				total_valor = obCalc.operacao(total_valor, valor, '+');
				
				//valor = converteStringEmNumero(valor);
				//total_valor += parseFloat(valor);
			}
		});

		$("#td_total_ob", window.opener.document).html( mascaraglobal('[.###],##',total_valor) );		
/*				
		if(!verificaNumeroInteiro(total_valor)){
			$("#td_total_ob", window.opener.document).html(mascaraglobal('[.###],##',total_valor));
			//$("#td_total_ob", window.opener.document).html(total_valor);
		}else{
			//alert(total_valor);
			$("#td_total_ob", window.opener.document).html(mascaraglobal('[.###]',total_valor)+",00");
			//$("#td_total_ob", window.opener.document).html(total_valor);
		}
*/		

//		if(total_valor != total){
	
		if( obCalc.comparar(total_valor, total, '=') == false ){
                        var valor =  $("#td_total_ob", window.opener.document).text(); 
			$("#td_obs_ob", window.opener.document).html("A soma das OBs é diferente do valor da nota fiscal.");
                        $("#tr_erro_calculo", window.opener.document).show();
                        $("#span_ftcinformado", window.opener.document).text(valor);
                        var total_contrato =  $("#ftcvalor", window.opener.document).val();
                        var diferenca = 0;
                        total_contrato = formata_valor_banco(total_contrato);
                        if(parseFloat(total_valor) > parseFloat(total_contrato)){
                           diferenca =   parseFloat(total_valor) - parseFloat(total_contrato);
                           $("#span_diferenca_ftcvalor", window.opener.document).css('color','red');
                           $("#span_diferenca_ftcvalor", window.opener.document).text('- '+formata_valor_moeda(diferenca));
                        }else{
                           diferenca =   parseFloat(total_contrato) - parseFloat(total_valor);  
                           $("#span_diferenca_ftcvalor", window.opener.document).css('color','blue'); 
                           $("#span_diferenca_ftcvalor", window.opener.document).text(formata_valor_moeda(diferenca)); 
                        }
                        
              }else{
			$("#td_obs_ob", window.opener.document).html("");
		}
             var  vl_total_contrato =  $("#vl_total_contrato", window.opener.document).text();
                 vl_total_contrato = formata_valor_banco(vl_total_contrato);
             if(parseFloat(total_valor) > parseFloat(vl_total_contrato))
             {
               $("#tr_erro_calculo_vlcontrato", window.opener.document).show();
               $("#valor_total_nf", window.opener.document).text($("#valor_total_nf", window.opener.document).text());
               $("#valor_total_contrato", window.opener.document).text($("#vl_total_contrato", window.opener.document).text());
             }else{
               $("#tr_erro_calculo_vlcontrato", window.opener.document).hide();
             } 
	}

	function verificaNumeroInteiro(n) 
	{
	   return ((typeof n==='number')&&(n%1===0));
	}

	function converteStringEmNumero(total)
	{
		total = total.replace(".", "");
		total = total.replace(".", "");
		total = total.replace(".", "");
		total = total.replace(".", "");
		total = total.replace(".", "");
		total = total.replace(".", "");
		total = total.replace(",", ".");
		total = total*1;
		return total;
	}

</script>

<?php 
monta_titulo('Ordem(s) Bancária(s)', 'Selecione os resgistros');
?>

<?php if($rs): ?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<thead>
		<tr>
			<th>&nbsp;</th>
			<th valign="top">Nº do empenho</th>
			<th valign="top">Nº da OB</th>
			<th valign="top">Data transação</th>
			<th valign="top">Observação</th>
			<th valign="top">Valor</th>
		</tr>
		</thead>
		<tbody>
		<?php foreach($rs as $x => $ob): ?>
			<?php $cor = $x%2 ? '#f0f0f0;' : 'white;'; ?>
			<tr style="background: <?php echo $cor; ?>">
				<td>
					<input type="checkbox" name="numob[]" id="ob_<?php echo trim($ob['ob']); ?>_<?php echo $ob['epsid']; ?>" class="checkob" value="<?php echo $ob['ob']; ?>_<?php echo $ob['valorob']; ?>_<?php echo $ob['datatransacao_banco']; ?>_<?php echo $ob['epsid']; ?>_<?php echo $ob['nu_empenho']; ?>" <?php // echo in_array($ob['ob'], $rsObCheck) ? 'checked' : ''; ?>/>
				</td>
				<td><?php echo $ob['nu_empenho']; ?></td>
				<td><?php echo $ob['ob']; ?></td>
				<td align="center"><?php echo $ob['datatransacao']; ?></td>
				<td><?php echo $ob['obsob']; ?></td>
				<td align="right"><?php echo formata_valor($ob['valorob']); ?></td>
			</tr>
		<?php endforeach; ?>
		</tbody>
	</table>
<?php else: ?>
	<center><b>Sem registros!</b></center>
<?php endif; ?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td bgcolor="#e9e9e9">
			<center><input type="button" value="Fechar" onclick="javascript:window.close();" /></center>
		</td>
	</tr>
</table>
<script>
$(document).ready(function(){	
	$.each($('.obsiafi', window.opener.document), function(i,v){
		$('#ob_' + $(v).attr('id') ).attr('checked', true);
	});
});
</script>
