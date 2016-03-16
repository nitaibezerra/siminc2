<?php
include '_funcoes_empenho_par_obras.php';

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

function listaEmpenhoObrasPar($dados) {
	global $db;
	if($dados['processo']) {
		$where[] = "empnumeroprocesso='".$dados['processo']."'";
	} elseif($dados['proid']) {
		$where[] = "proid='".$dados['proid']."'";
	}
	
	
	if($_SESSION['par_var']['esfera']=='estadual') {
		$where[] = "funid = 6";
	}else{
		$where[] = "funid = 1";
	}

	#Regras passados por Analista Thiago.
	#regras de perfil: é dado inico ao trabalho em 24/05/2012	
	$perfil = pegaPerfilGeral();	
	if(
		!(	in_array(PAR_PERFIL_EMPENHADOR, $perfil) ||
			in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
			in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)
		)		
	){
		$acaoConsulta = "''";
		$acaoCancela = "''";		
	}else{
		$acaoConsulta = "'<img src=../imagens/refresh2.gif style=cursor:pointer; onclick=consultarEmpenho('||e.empid||',\'' || trim(e.empnumeroprocesso) || '\');>'";
		$acaoCancela = "'<img src=../imagens/excluir.gif align=absmiddle style=cursor:pointer; onclick=cancelarEmpenho('||e.empid||',\'' || trim(e.empnumeroprocesso) || '\');>'";
	}		
	
	$sql = "SELECT '<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoEmpenho(\''||e.empid||'\', this);\">' as mais,
				   CASE WHEN e.empsituacao!='CANCELADO' THEN $acaoConsulta ELSE '&nbsp;' END as acao_consultar,
				   CASE WHEN e.empsituacao!='CANCELADO' THEN $acaoCancela ELSE '&nbsp;' END as acao_cancelar,
				   e.empcnpj, en.entnome, e.empprotocolo, e.empnumero, vve.vrlempenhocancelado as empvalorempenho, u.usunome, e.empsituacao FROM par.empenho e
			INNER JOIN par.processoobraspar p ON trim(e.empnumeroprocesso) = trim(p.pronumeroprocesso) and empcodigoespecie not in ('03', '13', '02', '04') and p.prostatus = 'A' and empstatus = 'A'
			inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
			LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
			LEFT JOIN entidade.entidade en ON en.entnumcpfcnpj=e.empcnpj
			LEFT JOIN entidade.funcaoentidade fun ON fun.entid=en.entid
			".(($where)?"WHERE ".implode(" AND ", $where):"");
	//dbg($sql,1);
	$cabecalho = array("&nbsp;","&nbsp;","&nbsp;","CNPJ","Entidade","Nº protocolo","Nº empenho","Valor empenho(R$)","Usuário criação","Situação empenho");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
$tabela_execucao = monta_tabela_execucao( TIPO_OBRAS_PAR );
monta_titulo( $titulo_modulo, $tabela_execucao );
?>

<script>
function enviarFormulario(){
	selectAllOptions( document.getElementById( 'tipoobra' ) );
	selectAllOptions( document.getElementById( 'grupoMunicipio' ) );
	formulario.submit();
}

/*function carregarListaEmpenhoObrasPar(pro, obj, inuid) {
	var tabela = obj.parentNode.parentNode.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode.parentNode;

	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex+1);
		var col0 = nlinha.insertCell(0);
		col0.innerHTML = "&nbsp;";
		var col1 = nlinha.insertCell(1);
		col1.id="listaempenhoprocesso_"+pro;
		col1.colSpan=8;
		carregarListaEmpenhoObrasPar2('', pro);
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex+1);	
	}
}

function carregarListaEmpenhoObrasPar2(empid,processo) {
	
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/empenhoParObras&acao=A",
		data: "requisicao=carregaEmpenhoPorProcesso&empid=" + empid + "&processo=" + processo,
		async: false,
		success: function(msg){
		if(!document.getElementById('listaempenhoprocesso')){
			document.getElementById('listaempenhoprocesso_' + processo).innerHTML = msg;
		}
		else{
			document.getElementById('listaempenhoprocesso').innerHTML = msg;
		}
	}
	});
	
}*/

function telaLogin( action ) {
	
	jQuery('input[type="text"], input[type="password"]').css('font-size', '14px');
	
	jQuery( "#div_login" ).show();
	jQuery( '#div_login' ).dialog({
		resizable: false,
		width: 500,
		modal: true,
		show: { effect: 'drop', direction: "up" },
		buttons: {
			'Ok': function() {
				if( action == 'consultar' ){
					consultarEmpenhoObrasParWS();
				}
				if( action == 'cancelar' ){
					cancelarEmpenhoObrasParWS();
				}
				window.location.reload();
			},
			'Fechar': function() {
				jQuery( this ).dialog( 'close' );
			}
		}
	});
}

function reduzirEmpenho(empid, processo, especie) {
	window.open('par.php?modulo=principal/reduzirEmpenhoObrasPar&acao=A&empid='+empid+'&processo='+processo+'&especie='+especie, 
				'reduzirEmpenhoObrasPar', 
				"height=400,width=800,scrollbars=yes,top=0,left=0" );
}	

</script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="./js/par.js"></script>
<!--JS para o componente de exibir o tolltip de dados do processo-->
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css" />
<form method="post" name="formulario" id="formulario">
	<input type="hidden" name="processo" id="processo" value="" />
	<input type="hidden" name="proid" id="proid" value="">
	<input type="hidden" name="wsempid" id="wsempid" value="" />
	<input type="hidden" name="ws_especie" id="ws_especie" value="" />
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
				<tr>
					<td class="SubTituloDireita" width="25%">Preid:</td>
                    <td><?php
                    echo campo_texto( 'preid', 'N', 'S', 'Preid', 10, 8, '[#]', '', '', '', '', '', "this.value=mascaraglobal('[#]',this.value)", $_REQUEST['preid']); ?></td>
                </tr>
                <tr>
                	<td class="SubTituloDireita">Obrid:</td>
                    <td><?php 
                	echo campo_texto( 'obrid', 'N', 'S', 'Obrid', 10, 8, '[#]', '', '', '', '', '', "this.value=mascaraglobal('[#]',this.value)", $_REQUEST['obrid']); ?></td>
                </tr>
				<tr>
                    
					<td class="SubTituloDireita">Número de Processo:</td>
					<td>
					<?php
						/*if( $_REQUEST['numeroprocesso'] ){
							$_REQUEST['numeroprocesso'] = substr($_REQUEST['numeroprocesso'],0,5) . "." .
														substr($_REQUEST['numeroprocesso'],5,6) . "/" .
														substr($_REQUEST['numeroprocesso'],11,4) . "-" .
														substr($_REQUEST['numeroprocesso'],15,2);
						}*/
					
						$filtro = simec_htmlentities( $_POST['empnumeroprocesso'] );
						$empnumeroprocesso = $filtro;
						echo campo_texto( 'empnumeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', ''); 
					?>
					</td>
				</tr>
					<tr>
						<td class="SubTituloDireita">Município:</td>
							<td>
							<?php 
								$filtro = simec_htmlentities( $_POST['municipio'] );
								$municipio = $filtro;
								echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Estado:</td>
							<td>
							<?php
								$estuf = $_POST['estuf'];
								$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
								$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Empenho:</td>
						<td>
							<?
								$empenhado = $_REQUEST['empenhado'];
								$arrEmpenho = array(
													array('codigo' => 1, 'descricao' => 'Empenhado Solicitado 100%'),
													array('codigo' => 2, 'descricao' => 'Nenhum Empenho Solicitado'),
													array('codigo' => 3, 'descricao' => 'Empenho Parcial')
												);
								$db->monta_combo( "empenhado", $arrEmpenho, 'S', 'Selecione', '', '' );
							?>
							<!--  Sim: <input <?php echo $_REQUEST['empenhado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="1" >
							Não: <input <?php echo $_REQUEST['empenhado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="2" >-->
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Esfera:</td>
						<td>
							<?php 
								$esfera = $_POST['esfera'];
								$arrEsfera = array(0 => array("codigo"=>"Municipal","descricao"=>"Municipal"), 1 => array("codigo"=>"Estadual","descricao"=>"Estadual"));
								$db->monta_combo( "esfera", $arrEsfera, 'S', 'Todas as esferas', '', '' ); 
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Ano do Processo:</td>
						<td>
							<?php
								$anoprocesso = $_POST['anoprocesso'];
								$sql = "select distinct  substring(prpnumeroprocesso,12,4) as codigo,  substring(prpnumeroprocesso,12,4) as descricao from par.processopar where substring(prpnumeroprocesso,12,4) <> '' and prpstatus = 'A' ";
								$db->monta_combo( "anoprocesso", $sql, 'S', 'Todos', '', '' );
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Grupo de Município:</td>
							<td>	
							<?php 
			
								$sql = "SELECT 
												tpmid as codigo, 
												tpmdsc as descricao
											FROM 
												territorios.tipomunicipio
											WHERE
												tpmstatus = 'A' 
												AND gtmid in ( 1,2 )
											ORDER BY
												descricao";
			
								combo_popup('grupoMunicipio', $sql, 'Selecione...', "400x400", 0, $_REQUEST['grupoMunicipio'], "", (($entcodent)?"N":"S"), false, false, 5, 400,
											null, null, '', '', $dados, true, false, '', '', Array('descricao'), Array('dsc') );
											
								?>															
						</td>
					</tr>
					<!--tr>
						<td valign="bottom" colspan="2">Resolução:</td>
					</tr>						
					<tr>
						<td valign="bottom" colspan="2">
							<?php 
//								$resid = $_POST['resid'];
//								$sql = "SELECT resid as codigo, resdescricao AS descricao FROM par.resolucao WHERE resstatus='A'";
//								$db->monta_combo( "resid", $sql, 'S', 'Todas as Resolução', '', '' ); 
							?>
						</td>
					</tr-->					
					
					<tr>
							<td class="SubTituloDireita">Tipo de Obra:</td>
							<td>
								<?php 

								$sql = "select 
												ptoclassificacaoobra as codigo, 
												ptodescricao as descricao
										from 
												obras.pretipoobra  
										where 
												ptostatus = 'A'";

								combo_popup('tipoobra', $sql, 'Selecione...', "400x400", 0, $_REQUEST['tipoobra'], "", (($entcodent)?"N":"S"), false, false, 5, 400,
											null, null, '', '', $dados, true, false, '', '', Array('descricao'), Array('dsc') );
											
								?>															
							</td>
						</tr>
                        <tr>
                            <td class="SubTituloDireita">Obras MUNICIPAIS com pendências:</td>
                            <td>
                                <input <?php echo $_REQUEST['pendenciaObras'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_1" value="1" >Sim
                                <input <?php echo $_REQUEST['pendenciaObras'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_2" value="2" >Não
                                <input <?php echo empty($_REQUEST['pendenciaObras']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" value=""   />Todos
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Obras ESTADUAIS com pendências:</td>
                            <td>
                                <input <?php echo $_REQUEST['pendenciaObrasUF'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_1" value="1" >Sim
                                <input <?php echo $_REQUEST['pendenciaObrasUF'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_2" value="2" >Não
                                <input <?php echo empty($_REQUEST['pendenciaObrasUF']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" value=""   />Todos
                            </td>
                        </tr>
<?php 					if( $db->testa_superuser() ){?>
                        <tr>
                            <td class="SubTituloDireita" style="color:red">Empenhos com Divergências:</td>
                            <td>
                                <input <?php echo $_REQUEST['empenhodivergente'] == "S" ? "checked='checked'" : "" ?> type="radio" name="empenhodivergente" value="S" >Sim
                                <input <?php echo $_REQUEST['empenhodivergente'] == "N" ? "checked='checked'" : "" ?> type="radio" name="empenhodivergente" value="N" >Não
                                <input <?php echo empty($_REQUEST['empenhodivergente']) ? "checked='checked'" : "" ?> type="radio" name="empenhodivergente" value=""   />Todos
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita" style="color:red">Processos com valor empenhado superior ao valor da obra:</td>
                            <td>
                                <input <?php echo $_REQUEST['vrlempenhomaior'] == "S" ? "checked='checked'" : "" ?> type="radio" name="vrlempenhomaior" value="S" >Sim
                                <input <?php echo $_REQUEST['vrlempenhomaior'] == "N" ? "checked='checked'" : "" ?> type="radio" name="vrlempenhomaior" value="N" >Não
                                <input <?php echo empty($_REQUEST['vrlempenhomaior']) ? "checked='checked'" : "" ?> type="radio" name="vrlempenhomaior" value="" />Todos
                            </td>
                        </tr>
<?php 					}?>
				<tr>
				<td class="SubTituloDireita"></td>
				<td>
					<input class="botao" type="hidden" name="pesquisar" value="Pesquisar"  />
					<input class="botao" type="button" name="btnpesquisar" value="Pesquisar" onclick="enviarFormulario();" />
					<input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location='par.php?modulo=principal/empenhoParObras&acao=A';" />
					<input class="botao" type="submit" name="exportarexcel" value="Exportar Excel" />
				</td>
			</td>
		</tr>
		
	</tbody>
</table>
</form>

<?php
//$whereest = array();
//$wheremun = array();
$where = array();
if($_REQUEST['pesquisar'] || $_REQUEST['exportarexcel']) {
	
	if($_REQUEST['empnumeroprocesso']){
		$_REQUEST['empnumeroprocesso'] = str_replace(".","", $_REQUEST['empnumeroprocesso']);
		$_REQUEST['empnumeroprocesso'] = str_replace("/","", $_REQUEST['empnumeroprocesso']);
		$_REQUEST['empnumeroprocesso'] = str_replace("-","", $_REQUEST['empnumeroprocesso']);
		$where[] = "p.pronumeroprocesso ilike '%".$_REQUEST['empnumeroprocesso']."%'";
	}	

	
if($_REQUEST['grupoMunicipio'][0]){
		$where[] = "tm.tpmid in (". implode(",",$_REQUEST['grupoMunicipio']). ")";
}

	if($_REQUEST['dopusucpfvalidacaogestor'] == 't') {
			$where[] = "dp.dopusucpfvalidacaogestor IS NOT NULL ";
				$where[] = "mdoid in (24,27,25,26)"; // Termo
		}
		if($_REQUEST['dopusucpfvalidacaogestor'] == 'f') {
			$where[] = "dp.dopusucpfvalidacaogestor IS NULL ";
			$where[] = "mdoid in (24,27,25,26)";
		}
	
		if($_REQUEST['dopusucpfvalidacaofnde'] == 't') {
			$where[] = "dp.dopusucpfvalidacaofnde IS NOT NULL ";
		}
		if($_REQUEST['dopusucpfvalidacaofnde'] == 'f') {
			$where[] = "dp.dopusucpfvalidacaofnde IS NULL ";
		}
		
		if($_REQUEST['anoprocesso']){
			$condicaoProcessoPar = " and  substring(prpnumeroprocesso,12,4) = '".$_REQUEST['anoprocesso']."'";
		}
	
//	ver(1, $_REQUEST['esfera'], d);
	$whereW = "Where ";
if($_REQUEST['esfera']) {
	switch($_REQUEST['esfera']) {
		case 'Municipal':
			$where[] = "(p.muncod is not null and p.muncod <> '')";
			break;
		case 'Estadual':
			$where[] = "(p.muncod is null or p.muncod = '')";
			break;
	}
}
	
	if($_REQUEST['municipio']) {
		//$wheremun[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
		//$whereest[] = "1=2";
		$where[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
	}
	if($_REQUEST['estuf']) {
		//$wheremun[] = "m.estuf='".$_REQUEST['estuf']."'";
		//$whereest[] = "p.estuf='".$_REQUEST['estuf']."'";
		$where[] = "(p.estuf='".$_REQUEST['estuf']."' or m.estuf='".$_REQUEST['estuf']."')";
	}
	
//	if($_REQUEST['empenhado'] == "1") {
//		//$wheremun[] = "(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) > 0";
//		//$whereest[] = "(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) > 0";
//		$where[] = "(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) > 0";
//	}
//	if($_REQUEST['empenhado'] == "2") {
//		//$wheremun[] = "(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) = 0";
//		//$whereest[] = "(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) = 0";
//		$where[] = "(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) = 0";
//	}

	if($_REQUEST['empenhado'] == "1") {
			$where[] = " p.proid IN(SELECT DISTINCT proid 
								FROM par.processoobraspar
								WHERE proid not in (
                -- PARCIALMENTE EMPENHADO
                               SELECT DISTINCT
                                               pop.proid
                               FROM par.processoobraspar  pop
                               INNER JOIN par.processoobrasparcomposicao popc ON popc.proid = pop.proid 
                               INNER JOIN par.empenho e ON e.empnumeroprocesso = pop.pronumeroprocesso AND empsituacao = '2 - EFETIVADO' and empcodigoespecie not in ('03', '13', '02', '04')
                               INNER JOIN par.empenhoobrapar eop ON eop.preid = popc.preid and eobstatus = 'A' and empstatus = 'A'
                               INNER JOIN obras.preobra po ON po.preid = eop.preid
                               WHERE pop.prostatus = 'A' and eop.eobpercentualemp < '100.00'   
                UNION ALL
                               -- NÃO EMPENHADO
                               SELECT DISTINCT proid
                               FROM par.processoobraspar 
                               WHERE pronumeroprocesso NOT IN (SELECT e.empnumeroprocesso 
                                                                                              FROM par.empenho e
                                                                                              INNER JOIN par.processoobraspar  pop ON e.empnumeroprocesso = pop.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and pop.prostatus = 'A' and empstatus = 'A'
                                                                                              WHERE (empsituacao = '2 - EFETIVADO'  OR empsituacao = '8 - SOLICITAÇÃO APROVADA')
                                                                                              ) 
)
			)";
		}
		if($_REQUEST['empenhado'] == "2") {
			$where[] = " p.proid IN (SELECT DISTINCT proid
								FROM par.processoobraspar 
								WHERE pronumeroprocesso NOT IN (SELECT e.empnumeroprocesso 
                                                               FROM par.empenho e
                                                               INNER JOIN par.processoobraspar  pop ON e.empnumeroprocesso = pop.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and pop.prostatus = 'A' and empstatus = 'A'
                                                               WHERE (empsituacao = '2 - EFETIVADO'  OR empsituacao = '8 - SOLICITAÇÃO APROVADA')
                                                               )
							) "; 
		}
		if($_REQUEST['empenhado'] == "3"){
		/*
			$where[] = " 3 = (SELECT 	case when pp.prptipoexecucao = 'C' then
											CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 99) THEN 1 --100% EMPENHADO
								            ELSE
								                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL 
								                        ELSE  2 --NÃO FOI EMPENHADO
								                        END
								            END
										else 
								            CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 100) THEN 1 --100% EMPENHADO
								            ELSE
								                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL 
								                        ELSE  2 --NÃO FOI EMPENHADO
								                        END
								            END
								        end AS empenho
							            
							FROM par.processoparcomposicao pc
							INNER JOIN par.processopar pp on pp.prpid = pc.prpid
							INNER JOIN par.subacaodetalhe s on s.sbdid = pc.sbdid
							LEFT JOIN par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso AND e.empsituacao = '2 - EFETIVADO'
							LEFT JOIN par.empenhosubacao es on es.empid = e.empid  and es.eobstatus = 'A' AND es.eobano = s.sbdano AND es.sbaid = s.sbaid 
							WHERE pc.ppcstatus = 'A' and pp.prpnumeroprocesso = p.prpnumeroprocesso
							GROUP BY pp.prpid, pp.prptipoexecucao) ";
		*/ 
			$where[] = " p.proid IN (SELECT DISTINCT
								                pop.proid
								FROM par.processoobraspar  pop
								INNER JOIN par.processoobrasparcomposicao popc ON popc.proid = pop.proid
								INNER JOIN par.empenho e ON e.empnumeroprocesso = pop.pronumeroprocesso AND empsituacao = '2 - EFETIVADO' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
								INNER JOIN par.empenhoobrapar eop ON eop.preid = popc.preid and eobstatus = 'A'
								INNER JOIN obras.preobra po ON po.preid = eop.preid
								WHERE pop.prostatus = 'A' and eop.eobpercentualemp < '100.00'
			)";
		} 
	
	if($_REQUEST['tipoobra'][0]) {
		$where[] = "protipo in ('". implode("','",$_REQUEST['tipoobra']). "')";
	}
	

    $pendenciaObras = $_REQUEST['pendenciaObras'];
    if($pendenciaObras == '1'){ // com pendencias
        $where[] = " iue.itrid = '2' ";
        $where[] = " p.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    if($pendenciaObras == '2'){ // sem pendencias
        $where[] = " iue.itrid = '2' ";
        $where[] = " p.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    $pendenciaObrasUF = $_REQUEST['pendenciaObrasUF'];
    if($pendenciaObrasUF == '1'){ // com pendencias
        $where[] = " iue.itrid = '1' ";
        $where[] = " p.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }

    if($pendenciaObrasUF == '2'){ // sem pendencias
        $where[] = " iue.itrid = '1' ";
        $where[] = " p.estuf not in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }

}

$perfil = pegaPerfilGeral();
//regras de acesso passada por Thiago em 24/05/2012
if((in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil)) || (in_array(PAR_PERFIL_CONSULTA_ESTADUAL, $perfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil)) || (in_array(PAR_PERFIL_CONSULTA, $perfil))){
	$acoes = "''";		
}else{
	$acoes = "'<center><img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/solicitacaoEmpenhoObrasPAR&acao=A&processo=' || p.pronumeroprocesso || '&proid='||p.proid||'\',\'Empenho\',\'scrollbars=yes,height=500,width=800,status=no,toolbar=no,menubar=no,location=no\');\"></center>'";
}	

$colunaDivergente =  '';
$filtroDivergente = '';
$joinDivergente = '';

if( $_REQUEST['empenhodivergente'] == 'S' ){
	$colunaDivergente	= ' coalesce(emp.temdivergente, 0) as temdivergente, ';
// 	$filtroDivergente 	= ' where temdivergente > 0';
	$where[] 			= " temdivergente > 0";
	$joinDivergente 	= "	left join(
		                   	select
		                   		count(empenho) as temdivergente, processo
	                 		from (
	                    		select 
							        coalesce(e.empvalorempenho, 0) as vrlempenho,
							        coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
							        (coalesce(e.empvalorempenho, 0) - coalesce(sum(es.eobvalorempenho), 0)) as diferenca,
							        e.empid as empenho,
							        e.empnumeroprocesso as processo,
							        e.empnumero as notaempenho,
							        e.empprotocolo as sequencial,
							        e.empcodigoespecie as especie,
							        'OBRA' as tipo
							    from
							        par.processoobraspar pp
							        inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso
							        left join par.empenhoobrapar es on es.empid = e.empid and es.eobstatus = 'A'
							    where
							        pp.prostatus = 'A'
							        and e.empstatus = 'A'
							        and e.empsituacao <> 'CANCELADO'
							    group by
							        e.empid,
							        e.empnumeroprocesso,
							        e.empnumero,
							        e.empcodigoespecie,
							        e.empprotocolo,
							        e.empvalorempenho
		                         ) as foo
		                         where
		                   			vrlempenho <> vrlempcomposicao
									and diferenca > 1
		                         group by processo
		                  	) emp on emp.processo = p.pronumeroprocesso ";
}
if( $_REQUEST['empenhodivergente'] == 'N' ){
	$colunaDivergente 	= ' coalesce(emp.temdivergente, 0) as temdivergente, ';
// 	$filtroDivergente 	= ' where temdivergente <= 0';
	$where[] = " temdivergente IS NULL";
	$joinDivergente 	= " left join(
		                  	select
		                 		count(empenho) as temdivergente, processo
		                 	from (
		                     	select 
							        coalesce(e.empvalorempenho, 0) as vrlempenho,
							        coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
							        (coalesce(e.empvalorempenho, 0) - coalesce(sum(es.eobvalorempenho), 0)) as diferenca,
							        e.empid as empenho,
							        e.empnumeroprocesso as processo,
							        e.empnumero as notaempenho,
							        e.empprotocolo as sequencial,
							        e.empcodigoespecie as especie,
							        'OBRA' as tipo
							    from
							        par.processoobraspar pp
							        inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso
							        left join par.empenhoobrapar es on es.empid = e.empid and es.eobstatus = 'A'
							    where
							        pp.prostatus = 'A'
							        and e.empstatus = 'A'
							        and e.empsituacao <> 'CANCELADO'
								group by
							        e.empid,
							        e.empnumeroprocesso,
							        e.empnumero,
							        e.empcodigoespecie,
							        e.empprotocolo,
							   		e.empvalorempenho
		                    	) as foo
		                   		where
		                   			vrlempenho <> vrlempcomposicao
									and diferenca > 1
		                     	group by processo
		                  	) emp on emp.processo = p.pronumeroprocesso ";
}

if( $_REQUEST['vrlempenhomaior'] == 'S' ){
	$where[] = "pre.preid in (select
								v.preid
							from
								par.vm_saldo_empenho_por_obra v
							    inner join obras.preobra p on p.preid = v.preid and p.prestatus = 'A'
							group by v.preid, p.prevalorobra, v.saldo
                            having (v.saldo > cast(p.prevalorobra as numeric(20,2)) and ((v.saldo - cast(p.prevalorobra as numeric(20,2))) > 1)))";
}
if( $_REQUEST['vrlempenhomaior'] == 'N' ){
	$where[] = "pre.preid not in (select
									v.preid
								from
									par.vm_saldo_empenho_por_obra v
								    inner join obras.preobra p on p.preid = v.preid and p.prestatus = 'A'
								group by v.preid, p.prevalorobra, v.saldo
								having (v.saldo > cast(p.prevalorobra as numeric(20,2)) and ((v.saldo - cast(p.prevalorobra as numeric(20,2))) > 1)))";
}

/*
if($wheremun[0] && $whereest[0] ){
	$filtrosWhereMun = " and ".implode(" AND ", $wheremun);
}else if($wheremun[0] && !$whereest[0]){
	$filtrosWhereMun = implode(" AND ", $wheremun);
}

if($wheremun[0] || $whereest[0]){
	$wheret = "WHERE";
}else{
	$wheret = "";
}
*/

if(!$_REQUEST['exportarexcel']){
	$acoes = " '<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenho(\''||p.pronumeroprocesso||'\', this, \'lista\');\"></center>' as mais,
			   '<center><img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A&processo=' || p.pronumeroprocesso || '&proid='||p.proid||'&inuid='||p.inuid||'\',\'Empenho\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acoes, 
               (
                  CASE WHEN (select count(*) as qtd from painel.dadosfinanceirosconvenios dfi where dfi.dfiprocesso = p.pronumeroprocesso) > 0 
                  THEN '<span class=\"processoDetalhes processo_detalhe\" >'
                  ELSE '<span class=\"processo_detalhe\" >'
                  END
                ) ||
				to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>' as pronumeroprocesso,";
	
	$colunaHabilita = "case when iue.iuesituacaohabilita = 'Habilitado' then
		              		'<center><img src=../imagens/workflow/1.png title=\"'||iue.iuesituacaohabilita||'\" style=cursor:pointer;></center>' 
		              	else 
		              		'<center><img src=../imagens/workflow/2.png title=\"'||iue.iuesituacaohabilita||'\" style=cursor:pointer;></center>' end as habilitado";
	$colunaSub = 'mais, acoes,';
} else {
	$acoes = "to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as pronumeroprocesso,";
	$colunaHabilita = 'iue.iuesituacaohabilita as habilitado';
	$colunaSub = '';
}

//$pendenciaObras = $_REQUEST['pendenciaObras'];
//if(!empty($pendenciaObras)) $where[] = buscarSelectProcessoPendencia($pendenciaObras);

	if($_REQUEST['preid']) {
		$where[] = " pre.preid = '{$_REQUEST['preid']}' ";
	}
    
	if($_REQUEST['obrid']) {
		$where[] = " obr.obrid = '{$_REQUEST['obrid']}' ";
	}
	
$sql = "select DISTINCT
			$colunaSub
			pronumeroprocesso,
			mundescricao,
			estuf,
			valorempenhado,
			qtdobrempenhadas,
			esfera,
			tiposistema,
			habilitado
		from(
		SELECT 	DISTINCT	 
			$acoes
                
			m.mundescricao, 
			$colunaDivergente
			CASE WHEN p.estuf IS NOT NULL THEN p.estuf ELSE m.estuf END as estuf,
			(SELECT sum(saldo) FROM par.v_saldo_empenho_por_obra WHERE processo = p.pronumeroprocesso) as valorempenhado,
			(select count(pc.preid) from par.processoobraspar po inner join par.processoobrasparcomposicao pc on pc.proid = po.proid where pc.pocstatus = 'A' and po.proid = p.proid) as qtdobrempenhadas,
			case when (p.muncod is null or p.muncod = '') then 'Estadual' else 'Municipal' end as esfera,
			case when p.sisid = 14 then 'Brasil-pró' else 'PAR' end as tiposistema,
			$colunaHabilita
		FROM par.processoobraspar 		p
		LEFT JOIN territorios.estado 	e ON e.estuf = p.estuf 
		LEFT JOIN territorios.municipio m 
			INNER JOIN territorios.muntipomunicipio mt ON mt.muncod = m.muncod
			INNER JOIN territorios.tipomunicipio 	tm ON tm.tpmid = mt.tpmid
		ON m.muncod = p.muncod
		INNER JOIN par.instrumentounidadeentidade 	iue on iue.inuid = p.inuid and iue.iuestatus = 'A' AND iue.iuedefault = true
		INNER JOIN par.processoobrasparcomposicao 	poc ON poc.proid = p.proid
		INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.prestatus = 'A'
		LEFT JOIN obras2.obras obr
			INNER JOIN workflow.documento 		doc2 ON doc2.docid = obr.docid AND tpdid = ".TPDID_OBJETO."
			INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid
		ON obr.obrid = pre.obrid AND obridpai IS NULL
		$joinDivergente
		WHERE p.prostatus = 'A' ".($where ? 'and ' . implode(' AND ', $where) : '')."	
		) as foo";

    if($_REQUEST['exportarexcel']){
        ob_clean();
        $cabecalho = array("Nº Processo", "Município","UF", /*"Tipo obra", "Nome da Obra",*/ "Valor empenhado(R$)","Qtd obras no processo","Esfera", 'Sistema', 'Entidade Habilitada');
        header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
        header ( "Pragma: no-cache" );
        header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
        header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
        header ( "Content-Description: MID Gera excel" );
        $db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%',$par2);
        exit;
    } else {
        // Só monta lista quando clicar em pesquisar
        if(isset($_REQUEST['estuf'])){
            $cabecalho = array("&nbsp;","&nbsp;","Nº Processo", "Município","UF", /*"Tipo obra", "Nome da Obra",*/ "Valor empenhado(R$)","Qtd obras no processo","Esfera", 'Sistema', 'Entidade Habilitada');
            //dbg($sql);
            //ver( simec_htmlentities($sql),d );
            $db->monta_lista($sql,$cabecalho,20,10,'N','center','N','formprocesso');
        }
    }
?>

<div id="div_login" title="Tela de Identificação" style="display: none; text-align: center;">
	<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php echo campo_texto("wsusuario", "S", "S", "Usuário", "25","","","","","","", "id='wsusuario'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" 
					onmouseover="MouseOver(this);" value="" size="26" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
	</table>
</div>

<div id="div_dialog" title="Histórico de Empenho" style="display: none; text-align: center;">
	<div style="padding:5px;text-align:justify;" id="mostra_dialog"></div>
</div>
<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth_consulta";

?>
