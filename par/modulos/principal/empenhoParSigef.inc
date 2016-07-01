<?php

if( $_POST['requisicao'] == 'reduzirValorManual' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$empid = $_POST['empid'];
	ver($_POST);
	$sql = "SELECT
				(par.retornacodigosubacao(s.sbaid))||'&nbsp;' as codigo,
				s.sbadsc,
			    par.recuperavalorvalidadossubacaoporano(s.sbaid, es.eobano) as vrlsubacao,
			    '<input type=text name=vrlcancelado id=vrlcancelado value='||ep.vrlcancelado||'>' as vrlcancelado,
			    es.eobano
			FROM
				par.empenho e
			    inner join par.empenhosubacao es on es.empid = e.empid and eobstatus = 'A' and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' and eobstatus = 'A' and empstatus = 'A'
			    inner join par.subacao s on s.sbaid = es.sbaid
			    inner join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie from par.empenho
		                    where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
		                    group by 
		                        empnumeroprocesso,
		                        empcodigoespecie,
		                        empidpai) as ep on ep.empidpai = e.empid
			 WHERE
			 	e.empid = $empid";
	
	$cabecalho = array("Codigo", "Subação", "Valor", "Valor Cancelado", "Ano");
	$db->monta_lista_simples($sql, $cabecalho, 60000, 1, 'N', '100%', 'S', true, false, false, true);	
	exit();
}
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Empenho de Cancelamento\Redução no SIGEF', '' );

if($_REQUEST['requisicao'] == 'pesquisar') {

	if($_REQUEST['numeroprocesso']) {
		$_REQUEST['numeroprocesso'] = str_replace(".","", $_REQUEST['numeroprocesso']);
		$_REQUEST['numeroprocesso'] = str_replace("/","", $_REQUEST['numeroprocesso']);
		$_REQUEST['numeroprocesso'] = str_replace("-","", $_REQUEST['numeroprocesso']);
		$where[] = "empnumeroprocesso = '".$_REQUEST['numeroprocesso']."'";
	}
	if($_REQUEST['anoprocesso']){
		$where[] = "substring(empnumeroprocesso,12,4) = '".$_REQUEST['anoprocesso']."'";
	}
}

$sql = "select distinct
		    empidpai,
			empnumeroprocesso,
		    cnpj,
		    usunome,
		    empprotocolo,
		    empcodigoespecie,
		    empdata,    
		    vrlcancelado,
		    vrlempenho,
		    empnumero,
			empsituacao,
			tipo,
            case when tipo = 'PAR' then 'Subações Genérico do PAR'
            	 when tipo = 'OBRA' then 'Obras PAR'
                 when tipo = 'PAC' then 'Obras do PAC'
            else '' end as tipoprocesso
		from(
		    select distinct
		        e.empnumeroprocesso,
		        formata_cpf_cnpj(e.empcnpj) as cnpj,
		        e.empprotocolo,
		        e.empcodigoespecie,
		        e.empdata,
		        u.usunome,
		        ep.empidpai,
		        ep.vrlcancelado,
		        sum(vve.vrlempenhocancelado) as vrlempenho,
		        e.empnumero,
				e.empsituacao,
                case when (select count(eobid) from par.empenhosubacao where empid = ep.empidpai and eobstatus = 'A') > 0 then 'PAR'
                	 when (select count(eobid) from par.empenhoobrapar where empid = ep.empidpai and eobstatus = 'A') > 0 then 'OBRA'
                	 when (select count(eobid) from par.empenhoobra o where empid = ep.empidpai and eobstatus = 'A') > 0 then 'PAC'
				else '' end tipo
		    from
		        par.empenho e
				inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid and empstatus = 'A'
		        inner join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie from par.empenho
		                    where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
		                    group by 
		                        empnumeroprocesso,
		                        empcodigoespecie,
		                        empidpai) as ep on ep.empidpai = e.empid
		        left join seguranca.usuario u ON u.usucpf=e.usucpf
			where
            	ep.empidpai in (select
		                              empid
		                          from(
		                              select empid from par.empenhosubacao where eobstatus = 'A' group by empid having count(sbaid) > 1
		                              union all
		                              select empid from par.empenhoobrapar where eobstatus = 'A' group by empid having count(preid) > 1
		                              union all
		                              select empid from par.empenhoobra where eobstatus = 'A' group by empid having count(preid) > 1
		                          ) as fo1
								)
				and e.empcodigoespecie not in ('03', '13', '02', '04')
		    group by
		        e.empnumeroprocesso,
		        e.empcnpj,
		        e.empprotocolo,
		        e.empcodigoespecie,
		        e.empdata,
		        ep.empidpai,
		        ep.vrlcancelado,
		        e.empnumero,
		        u.usunome,
				e.empsituacao
		) as foo
			where
		    	vrlempenho > vrlcancelado
				".(($where)? ' and '.implode(" AND ", $where):"")."
		order by empnumeroprocesso";
	
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();		
	?>	
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
		<tr>
			<td class="SubTituloDireita">Número de Processo:</td>
			<td colspan="3">
			<?php
				$numeroprocesso = simec_htmlentities( $_POST['numeroprocesso'] );
				echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', ''); 
			?>
			</td>
			
		</tr>
		<tr>
			<td class="subtitulodireita">Ano do Processo:</td>
			<td colspan="3">
				<?php
				$anoprocesso = $_POST['anoprocesso'];
				$sql = "select distinct  substring(prpnumeroprocesso,12,4) as codigo,  substring(prpnumeroprocesso,12,4) as descricao from par.processopar where substring(prpnumeroprocesso,12,4) <> '' and prpstatus = 'A' ";
				$db->monta_combo( "anoprocesso", $sql, 'S', 'Todos', '', '' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="4" style="text-align: center;">
				<input class="botao" type="button" name="pesquisar" id= "pesquisar" value="Pesquisar" onclick="submeteFormulario();">
				<input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location='par.php?modulo=principal/empenhoParSigef&acao=A';" />
			</td>
		</tr>
	</table>
</form>
<table align="center" border="0"  class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="subtitulodireita" style="text-align: center;" colspan="2">Lista de Empenhos com valor cancelado menor</td>
	</tr>
<?php if( $arrDados ){ ?>
	<tr>
		<td>
			<table width="100%" align="left" cellspacing="0" cellpadding="2" border="0" class="listagem" style="color:333333;">
			<thead>
				<tr>
					<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="05%">Ações</td>
					<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">CNPJ</td>
					<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%">Processo</td>
					<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="06%">Nº protocolo</td>
					<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="06%">Nº empenho</td>
					<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Valor empenho(R$)</td>
					<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Valor Cancelado(R$)</td>
					<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="28%">Usuário criação</td>
					<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Situação empenho</td>
					<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="13%">Tipo de Processo</td>
				</tr>
			</thead>
			<tbody>
		<?php
			foreach ($arrDados as $key => $v ) {
				$key % 2 ? $cor = "#dedfde" : $cor = "";
				?>
				<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
					<td valign="top" align="center">
						<img src=../imagens/ico_config_i.gif align=absmiddle style=cursor:pointer; title="Reduzir Valor Manual" onclick="reduzirValor(<?php echo $v['empidpai']; ?>, '<?php echo $v['tipo']; ?>')"></td>						
					<td valign="top" title="CNPJ"><?php echo $v['cnpj'];?></td>
					<td valign="top" title="CNPJ"><?php echo formatarProcesso($v['empnumeroprocesso']);?></td>
					<td valign="top" title="Nº protocolo"><?php echo $v['empprotocolo'];?></td>
					<td valign="top" title="Nº empenho"><?php echo $v['empnumero'];?></td>
					<td valign="top" align="right" title="Valor empenho(R$)" style="color:#999999;"><?php echo number_format( $v['vrlempenho'], 2, ',', '.' );?></td>
					<td valign="top" align="right" title="Valor Cancelado(R$)" style="color:#999999;"><?php echo number_format( $v['vrlcancelado'], 2, ',', '.' );?></td>
					<td valign="top" title="Usuário criação"><?php echo $v['usunome'];?></td>
					<td valign="top" title="Situação empenho"><?php echo $v['empsituacao'];?></td>
					<td valign="top" title="Situação empenho"><?php echo $v['tipoprocesso'];?></td>
				</tr>
		<?php }?>
			</tbody>
		</table>
		</td>
		</tr>
		<tr>
			<td>
				<table width="100%" align="left" cellspacing="0" cellpadding="2" border="0" class="listagem">
				<tbody>
					<tr bgcolor="#ffffff">
						<td><b>Total de Registros: <?php echo sizeof($arrDados); ?></b></td></tr><tr>
					</tr>
				</tbody>
				</table>
			</td>
		</tr>
<?php } else {
	?>
	<tr>
		<td>
			<table width="100%" align="left" cellspacing="0" cellpadding="2" border="0" class="listagem">
			<tbody>
				<tr bgcolor="#ffffff">
					<td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
				</tr>
			</tbody>
			</table>
		</td>
	</tr>
<?php } ?>
	</table>
	
<div id="div_dialog" title="Histórico de Empenho" style="display: none; text-align: center;">
	<form name="formDivDialog" id="formDivDialog" action="" enctype="multipart/form-data" method="post">
		<div style="padding:5px;text-align:justify;" id="mostra_dialog"></div>
	</form>
</div>

<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>

<script type="text/javascript">

function submeteFormulario(){
	$('#requisicao').val('pesquisar');
	$('#formulario').submit();
}
	
function reduzirValor( empid, tipo ) {
	
	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "requisicao=reduzirValorManual&empid="+empid+'&tipo='+tipo,
		async: false,
		success: function(msg){
			jQuery( "#div_dialog" ).show();
			jQuery( "#mostra_dialog" ).html(msg);

			jQuery('input[type="text"], input[type="password"]').css('font-size', '14px');
			
			jQuery( '#div_dialog' ).dialog({
					resizable: false,
					width: 900,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
					'Fechar': function() {
						jQuery( this ).dialog( 'close' );
					}
				}
			});
		}
	});
}
</script>