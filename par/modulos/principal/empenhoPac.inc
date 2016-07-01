<?php
include '_funcoes_empenho.php';

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
$tabela_execucao = monta_tabela_execucao( TIPO_OBRAS_PAC );
monta_titulo( $titulo_modulo, $tabela_execucao );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="./js/par.js"></script>
<!--JS para o componente de exibir o tolltip de dados do processo-->
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css" />

<script type="text/javascript">
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
					consultarEmpenhoWS();
				}
				if( action == 'cancelar' ){
					cancelarEmpenhoWS();
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
	window.open('par.php?modulo=principal/reduzirEmpenhoPAC&acao=A&empid='+empid+'&processo='+processo+'&especie='+especie, 
			    'reduzirEmpenho', 
			    "height=400,width=800,scrollbars=yes,top=0,left=0" );
}
</script>

<form method="post" name="formulario" id="formulario">
	<input type="hidden" name="processo" id="processo" value="" />
	<input type="hidden" name="wsempid" id="wsempid" value="" />
	<input type="hidden" name="proid" id="proid" value="" />
	<input type="hidden" name="ws_especie" id="ws_especie" value="" />
				
	<table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tbody>
                        <tr>
                            <td class="SubTituloDireita">Preid:</td>
                            <td><?php
                            echo campo_texto( 'preid', 'N', 'S', 'Preid', 10, 8, '[#]', '', '', '', '', '', "this.value=mascaraglobal('[#]',this.value)", $_REQUEST['preid']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Obrid:</td>
                            <td><?php 
                            echo campo_texto( 'obrid', 'N', 'S', 'Obrid', 10, 8, '[#]', '', '', '', '', '', "this.value=mascaraglobal('[#]',this.value)",  $_REQUEST['obrid']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Número de Processo:</td>
                                <td colspan="3">
                                <?php
                                        $empnumeroprocesso = simec_htmlentities( $_REQUEST['empnumeroprocesso'] );

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
                                <td class="SubTituloDireita">Resolução:</td>
                                <td>
                                        <?php 
                                                $resid = $_POST['resid'];
                                                $sql = "select resid as codigo, resdescricao as descricao from par.resolucao where resstatus='A'";
                                                $db->monta_combo( "resid", $sql, 'S', 'Todas as Resolução', '', '' ); 
                                        ?>
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
                                <td class="SubTituloDireita">Tipo de Obra:</td>
                                <td>
                                        <input <?php echo $_REQUEST['tipoobra'] == "P" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="P" />Proinfancia
                                        <input <?php echo $_REQUEST['tipoobra'] == "Q" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="Q" />Quadra
                                        <?if(empty($_REQUEST['tipoobra'])){ ?>
                                                <input <?php echo $_REQUEST['tipoobra'] == "T" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="T" checked="checked" />Todos
                                        <?}else{ ?>
                                                <input <?php echo $_REQUEST['tipoobra'] == "T" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="T" />Todos
                                        <?} ?>
                                </td>
                        </tr>
                        <tr>
                                <td class="SubTituloDireita">Empenho:</td>
                                <td>
                                        <input <?php echo $_REQUEST['empenhado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="1" />Empenho Solicitado
                                        <input <?php echo $_REQUEST['empenhado'] == "4" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="4" />Empenho Solicitado Parcialmente
                                        <input <?php echo $_REQUEST['empenhado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="2" />Empenho Não Solicitado
                                        <?if(empty($_REQUEST['empenhado'])){ ?>
                                                <input <?php echo $_REQUEST['empenhado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="3" checked="checked" />Todos
                                        <?}else{ ?>
                                                <input <?php echo $_REQUEST['empenhado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="3" />Todos
                                        <?} ?>
                                </td>
                        </tr>
                        <tr>
                                <td class="SubTituloDireita">Metodologia Inovadora:</td>
                                <td>
                                        <input <?php echo $_REQUEST['metodologia'] == "1" ? "checked='checked'" : "" ?> type="checkbox" name="metodologia" value="1" />
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
							<td class="subtitulodireita"></td>
							<td colspan="3">
							<input type="submit" name="pesquisar" value="Pesquisar" />
							<input type="button" name="todos" value="Ver todos" onclick="window.location='par.php?modulo=principal/empenhoPac&acao=A';" />
							<input type="submit" name="exportarexcel" value="Exportar Excel"/>
						</tr>
			</tr>
		</tbody>
	</table>
</form>
<?php
if($_REQUEST['pesquisar'] || $_REQUEST['exportarexcel'] ) {
	
	if($_REQUEST['empnumeroprocesso']){
		$_REQUEST['empnumeroprocesso'] = str_replace(".","", $_REQUEST['empnumeroprocesso']);
		$_REQUEST['empnumeroprocesso'] = str_replace("/","", $_REQUEST['empnumeroprocesso']);
		$_REQUEST['empnumeroprocesso'] = str_replace("-","", $_REQUEST['empnumeroprocesso']);
		$wheremun[] = "p.pronumeroprocesso ilike '%".$_REQUEST['empnumeroprocesso']."%'";
		$whereest[] = "p.pronumeroprocesso ilike '%".$_REQUEST['empnumeroprocesso']."%'";
	}	

	if($_REQUEST['esfera']) {
		switch($_REQUEST['esfera']) {
			case 'Municipal':
				$whereest[] = "1=2";
				$wheremun[] = "p.muncod IS NOT NULL";
				break;
			case 'Estadual':
				$wheremun[] = "1=2";
				$whereest[] = "p.muncod IS NULL";
				break;
		}
	}
	
	if($_REQUEST['municipio']) {
		$wheremun[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
		$whereest[] = "1=2";
	}
	if($_REQUEST['estuf']) {
		$wheremun[] = "m.estuf='".$_REQUEST['estuf']."'";
		$whereest[] = "p.estuf='".$_REQUEST['estuf']."'";
	}
        
        $sqlValorEmpenho = "SELECT
                                SUM(vve.vrlempenhocancelado)
                            FROM par.empenho e
                            INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = e.empid
                            WHERE e.empstatus = 'A'
                                AND e.empcodigoespecie NOT IN ('03', '13', '02', '04')
                                AND e.empnumeroprocesso=p.pronumeroprocesso
                                AND vve.vrlempenhocancelado > 0";
	if($_REQUEST['empenhado'] == "1") {
		$wheremun[] = "({$sqlValorEmpenho}) > 0";
		$whereest[] = "({$sqlValorEmpenho}) > 0";
	}
	if($_REQUEST['empenhado'] == "2") {
		$wheremun[] = "({$sqlValorEmpenho}) = 0";
		$whereest[] = "({$sqlValorEmpenho}) = 0";
	}
	if($_REQUEST['empenhado'] == "4") {
		$wheremun[] = "(SELECT 
                                    round(
                                        (SELECT 
                                            sum(po.prevalorobra) 
                                        FROM
                                            obras.preobra po
                                        inner join par.processoobraspaccomposicao popc1 on popc1.preid = po.preid and popc1.pocstatus = 'A'
                                        INNER JOIN par.processoobra pro ON pro.proid = popc1.proid and pro.prostatus = 'A'
                                        WHERE
                                            pronumeroprocesso = p.pronumeroprocesso)
                                        -
                                        sum(eob.eobvalorempenho)
                                    )
                                FROM
                                    obras.preobra po
                                INNER JOIN par.processoobraspaccomposicao 	popc1 on popc1.preid = po.preid and popc1.pocstatus = 'A'
                                INNER JOIN par.processoobra 			pro ON pro.proid = popc1.proid and pro.prostatus = 'A'
                                INNER JOIN par.empenhoobra 			eob ON eob.preid = po.preid and eobstatus = 'A'
                                WHERE
                                    pronumeroprocesso = p.pronumeroprocesso) > 0";
		$whereest[] = "(SELECT 
                                    round(
                                        (SELECT 
                                            sum(po.prevalorobra) 
                                        FROM
                                            obras.preobra po
                                        inner join par.processoobraspaccomposicao popc1 on popc1.preid = po.preid and popc1.pocstatus = 'A'
                                        INNER JOIN par.processoobra pro ON pro.proid = popc1.proid and pro.prostatus = 'A'
                                        WHERE
                                            pronumeroprocesso = p.pronumeroprocesso)
                                        -
                                        sum(eob.eobvalorempenho)
                                    )
                                FROM
                                    obras.preobra po
                                inner join par.processoobraspaccomposicao 	popc1 on popc1.preid = po.preid and popc1.pocstatus = 'A'
                                INNER JOIN par.processoobra 			pro ON pro.proid = popc1.proid and pro.prostatus = 'A'
                                INNER JOIN par.empenhoobra 			eob ON eob.preid = po.preid and eobstatus = 'A'
                                WHERE
                                        pronumeroprocesso = p.pronumeroprocesso) > 0";
	}
	if($_REQUEST['tipoobra'] == "P") {
		$wheremun[] = "protipo = 'P'";
		$whereest[] = "protipo = 'P'";
	}
	if($_REQUEST['tipoobra'] == "Q") {
		$wheremun[] = "protipo <> 'P'";
		$whereest[] = "protipo <> 'P'";
	}
	if($_REQUEST['resid']) {
		$wheremun[] = "res.resid = '".$_REQUEST['resid']."'";
		$whereest[] = "res.resid = '".$_REQUEST['resid']."'";
	}
	if($_REQUEST['metodologia']) {
		$wheremun[] = "p.proid IN (SELECT poc.proid FROM par.processoobra po
	  				   INNER JOIN par.processoobraspaccomposicao poc ON poc.proid = po.proid and poc.pocstatus = 'A'
	  				   INNER JOIN obras.preobra p ON p.preid = poc.preid 
	  				   WHERE po.prostatus = 'A' and p.ptoid IN (42,43,44,45))";
		$whereest[] = "p.proid IN (SELECT poc.proid FROM par.processoobra po
	  				   INNER JOIN par.processoobraspaccomposicao poc ON poc.proid = po.proid and poc.pocstatus = 'A'
	  				   INNER JOIN obras.preobra p ON p.preid = poc.preid 
	  				   WHERE po.prostatus = 'A' and p.ptoid IN (42,43,44,45))";
	}

    $pendenciaObras = $_REQUEST['pendenciaObras'];
    if($pendenciaObras == '1'){ // com pendencias
        // Negando estadual
        $whereest[] = "1=2";
        $wheremun[] = " p.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    if($pendenciaObras == '2'){ // sem pendencias
        // Negando estadual
        $whereest[] = "1=2";
        $wheremun[] = " p.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    $pendenciaObrasUF = $_REQUEST['pendenciaObrasUF'];
    if($pendenciaObrasUF == '1'){ // com pendencias
        // Negando municipal
        $wheremun[] = "1=2";
        $whereest[] = " p.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }

    if($pendenciaObrasUF == '2'){ // sem pendencias
        // Negando municipal
        $wheremun[] = "1=2";
        $whereest[] = " p.estuf not in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }
    
    $colunaDivergente =  '';
    $filtroDivergente = '';
    $joinDivergente = '';
    
    if( $_REQUEST['empenhodivergente'] == 'S' ){
    	$colunaDivergente 	= ' emp.temdivergente as temdivergente, ';
// 		$filtroDivergente 	= ' where temdivergente > 0';
        $wheremun[] 		= "	emp.temdivergente > 0";
        $whereest[] 		= "	emp.temdivergente > 0";
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
								        'PAC' as tipo
								    from
								        par.processoobra pp
								        inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso
								        left join par.empenhoobra es on es.empid = e.empid and es.eobstatus = 'A'
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
		$colunaDivergente 	= ' emp.temdivergente as temdivergente, ';
// 		$filtroDivergente = ' where temdivergente <= 0';
        $wheremun[] 		= "	emp.temdivergente IS NULL";
        $whereest[] 		= "	emp.temdivergente IS NULL";
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
								        'PAC' as tipo
								    from
								        par.processoobra pp
								        inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso
								        left join par.empenhoobra es on es.empid = e.empid and es.eobstatus = 'A'
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
    	$wheremun[] = "pre.preid in (select
								v.preid
							from
								par.vm_saldo_empenho_por_obra v
							    inner join obras.preobra p on p.preid = v.preid and p.prestatus = 'A'
							group by v.preid, p.prevalorobra, v.saldo
							having (v.saldo > cast(p.prevalorobra as numeric(20,2)) and ((v.saldo - cast(p.prevalorobra as numeric(20,2))) > 1))) ";
    	$whereest[] = "pre.preid in (select
									v.preid
								from
									par.vm_saldo_empenho_por_obra v
								    inner join obras.preobra p on p.preid = v.preid and p.prestatus = 'A'
								group by v.preid, p.prevalorobra, v.saldo
								having (v.saldo > cast(p.prevalorobra as numeric(20,2)) and ((v.saldo - cast(p.prevalorobra as numeric(20,2))) > 1)))";
    }
    if( $_REQUEST['vrlempenhomaior'] == 'N' ){
    	$wheremun[] = "pre.preid not in (select
									v.preid
								from
									par.vm_saldo_empenho_por_obra v
								    inner join obras.preobra p on p.preid = v.preid and p.prestatus = 'A'
								group by v.preid, p.prevalorobra, v.saldo
								having (v.saldo > cast(p.prevalorobra as numeric(20,2)) and ((v.saldo - cast(p.prevalorobra as numeric(20,2))) > 1)))";
    	$whereest[] = "pre.preid not in (select
									v.preid
								from
									par.vm_saldo_empenho_por_obra v
								    inner join obras.preobra p on p.preid = v.preid and p.prestatus = 'A'
								group by v.preid, p.prevalorobra, v.saldo
								having (v.saldo > cast(p.prevalorobra as numeric(20,2)) and ((v.saldo - cast(p.prevalorobra as numeric(20,2))) > 1)))";
    }
}

if($_REQUEST['exportarexcel']){
	$cabecalho = array("Nº Processo","Resolução", "Município","UF", "Tipo obra","Valor empenhado(R$)","Qtd obras no processo","Esfera");
	
	$colunaAcoes = "substring(p.pronumeroprocesso from 1 for 5)||'.'||substring(p.pronumeroprocesso from 6 for 6)||'/'||
					substring(p.pronumeroprocesso from 12 for 4)||'-'||substring(p.pronumeroprocesso from 16 for 2) as numeroprocesso,";
	$acoes = '';
	$colunaUnion = "to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') numeroprocesso,";
	$colunaSub = '';
} else {
	$cabecalho = array("&nbsp;","&nbsp;","Nº Processo","Resolução", "Município","UF", "Tipo obra","Valor empenhado(R$)","Qtd obras no processo","Esfera");
	$acoes = "'<center><img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/solicitacaoEmpenho&acao=A&processo=' || p.pronumeroprocesso || '&proid='||p.proid||'\',\'Empenho\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>'";

 	$colunaAcoes = "'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenho(\''||p.pronumeroprocesso||'\', this, \'lista\');\"></center>' as mais,
			   		$acoes as acoes, 
 					(
		            CASE WHEN (select count(*) as qtd from painel.dadosfinanceirosconvenios dfi where dfi.dfiprocesso = p.pronumeroprocesso) > 0 THEN
		            	'<span class=\"processoDetalhes processo_detalhe\" >'
					ELSE
		            	'<span class=\"processo_detalhe\" >'
					END
		            ) ||
					substring(p.pronumeroprocesso from 1 for 5)||'.'||substring(p.pronumeroprocesso from 6 for 6)||'/'||
					substring(p.pronumeroprocesso from 12 for 4)||'-'||substring(p.pronumeroprocesso from 16 for 2) || '</span>' as numeroprocesso,";
			   
	$colunaUnion = "'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenho(\''||p.pronumeroprocesso||'\', this, \'lista\');\"></center>' as mais,
			   		'<center><img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/solicitacaoEmpenho&acao=A&processo=' || p.pronumeroprocesso || '&proid='||p.proid||'\',\'Empenho\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acoes, 
					'<span class=\"processo_detalhe\" >' || to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>' as numeroprocesso,";

	$colunaSub = 'mais, acoes,';
}


if($_REQUEST['preid']) {
    $wheremun[] = " pre.preid = '{$_REQUEST['preid']}' ";
    $whereest[] = " pre.preid = '{$_REQUEST['preid']}' ";
}
$joinObra = '';
if($_REQUEST['obrid']) {
    $wheremun[] = " obr.obrid = '{$_REQUEST['obrid']}' ";
    $whereest[] = " obr.obrid = '{$_REQUEST['obrid']}' ";
    $joinObra = "LEFT JOIN obras2.obras obr
					INNER JOIN workflow.documento doc2 ON doc2.docid = obr.docid AND tpdid = ".TPDID_OBJETO."
					INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid
		         ON obr.obrid = pre.obrid AND obridpai IS NULL";
}

$wheremun[] = " m.mundescricao IS NOT NULL ";
$whereest[] = " p.estuf IS NOT NULL AND p.muncod IS NULL ";

$wheremun[] = " p.prostatus = 'A' ";
$whereest[] = " p.prostatus = 'A' ";

$sql = "SELECT distinct
			$colunaSub
			numeroprocesso,
			resolucao,
			mundescricao,
			estuf,
			tipoobra,
			valorempenhado,
			qtdobrprocesso,
			esfera
		FROM
		(
			(SELECT distinct 
				$colunaAcoes 
				trim(res.resdescricao) as resolucao, 
				m.mundescricao, 
				m.estuf, 
				$colunaDivergente
				CASE WHEN protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
				(SELECT sum(saldo) FROM par.vm_saldo_empenho_por_obra WHERE processo = p.pronumeroprocesso) as valorempenhado,
				qtd.qtdobrprocesso,
				'Municipal' as esfera 
			FROM par.processoobra p 
			LEFT JOIN par.processoobraspaccomposicao 	poc ON poc.proid = p.proid AND poc.pocstatus = 'A'
			LEFT JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.prestatus = 'A'
			$joinObra
		    left join(
				  select count(pc.preid) as qtdobrprocesso, po.pronumeroprocesso 
				  from par.processoobra po 
				      inner join par.processoobraspaccomposicao pc on pc.proid = po.proid 
				  where pc.pocstatus = 'A' and po.prostatus = 'A'
				  group by po.pronumeroprocesso
		    ) qtd on qtd.pronumeroprocesso = p.pronumeroprocesso
			LEFT JOIN par.resolucao 					res ON res.resid = p.resid
			INNER JOIN territorios.municipio 			m   ON m.muncod = p.muncod 
			$joinDivergente
			where 1=1 
			".(($wheremun)?"and ".implode(" AND ", $wheremun):"")."
			)
			UNION ALL (
			SELECT distinct
				$colunaUnion 
				trim(res.resdescricao) as resolucao, 
				'<center>-</center>' as mundescricao, 
				p.estuf, 
				$colunaDivergente
				CASE WHEN protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
				(SELECT sum(saldo) FROM par.vm_saldo_empenho_por_obra WHERE processo = p.pronumeroprocesso) as valorempenhado,
				qtd.qtdobrprocesso,
				   'Estadual' as esfera
			FROM par.processoobra p
			LEFT JOIN par.processoobraspaccomposicao 	poc ON(poc.proid = p.proid) and poc.pocstatus = 'A'
			LEFT JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.prestatus = 'A'
			$joinObra
		    left join(
				  select count(pc.preid) as qtdobrprocesso, po.pronumeroprocesso 
				  from par.processoobra po 
				      inner join par.processoobraspaccomposicao pc on pc.proid = po.proid 
				  where pc.pocstatus = 'A' and po.prostatus = 'A'
				  group by po.pronumeroprocesso
		    ) qtd on qtd.pronumeroprocesso = p.pronumeroprocesso
			LEFT JOIN par.resolucao 					res ON res.resid = p.resid
			$joinDivergente
			where 1=1 
			".(($whereest)?"and ".implode(" AND ", $whereest):"")."
			)
		) as foo
		ORDER BY
			numeroprocesso";

    if($_REQUEST['exportarexcel']) {
        ob_clean();
        header('content-type: text/html; charset=ISO-8859-1');
        $formato = array('', '', '', '', '', 'n', '', '');
        $db->sql_to_excel($sql, 'relEmendasPTA', $cabecalho, $formato);

        /*header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
        header ( "Pragma: no-cache" );
        header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
        header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
        header ( "Content-Description: MID Gera excel" );
        $db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'N','100%',$par2);*/
        exit;
    } else {
        // Só monta lista quando clicar em pesquisar
        if(isset($_REQUEST['estuf'])){
        //dbg($sql);
//         	ver(simec_htmlentities($sql),d);
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
/*
?>
<style>
	.popup_alerta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php echo campo_texto("ws_usuario_consulta","S","S","Usuário","22","","","","","","","id='ws_usuario_consulta'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha_consulta" name="ws_senha_consulta">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="hidden" name="ws_empid" id="ws_empid" value="" />
				<input type="hidden" name="ws_processo" id="ws_processo" value="" />
				<input type="button" name="btn_enviar" onclick="consultarEmpenhoWS()" value="ok" />
				<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>
<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth_cancela";
?>
<style>
	.popup_alerta_cancela{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta_cancela <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" class="SubtituloDireita">Usuário:</td>
				<td><?php echo campo_texto("ws_usuario_cancela","S","S","Usuário","22","","","","","","","id='ws_usuario_cancela'") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha_cancela" name="ws_senha_cancela">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td align="center" bgcolor="#D5D5D5" colspan="2">
					<input type="hidden" name="ws_empid" id="ws_empid" value="" />
					<input type="hidden" name="ws_processo" id="ws_processo" value="" />
					<input type="button" name="btn_enviar" onclick="cancelarEmpenhoWS()" value="ok" />
					<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
				</td>
			</tr>
		</table>
	</div>
</div>
<?php
*/
?>