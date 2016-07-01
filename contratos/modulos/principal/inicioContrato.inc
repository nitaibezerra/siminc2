<?php

// Cria uma função que retorna o timestamp de uma data no formato DD/MM/AAAA
function geraTimestamp($data) {

    $partes = explode('/', $data);
    return mktime(0, 0, 0, $partes[1], $partes[0], $partes[2]);
}

function dif_data($dtini, $dtfim) {
    $time_inicial = geraTimestamp($dtini);
    if ($dtfim) {
        $time_final = geraTimestamp($dtfim);
    } else {
        $time_final = geraTimeStamp(date('d/m/Y'));
    }

    $diferenca = $time_final - $time_inicial;
    return (int)floor( $diferenca / (60 * 60 * 24));
}

if($_REQUEST['consulta'] == '')
	$_REQUEST['consulta'] = 'html';

//verifica perfis
$perfis 		= arrayPerfil();
$superPerfis 	= array(PERFIL_ADMINISTRADOR,PERFIL_SUPER_USUARIO,PERFIL_CONSULTA_GERAL);
$arIntersec 	= array_intersect($superPerfis, $perfis);

// REMOVER
if ( $_REQUEST['remover'] ) 
{
	$ctrid = (integer) $_REQUEST['remover'];  
	$sql = "update contratos.ctcontrato SET ctrstatus = 'I' WHERE ctrid = " . $ctrid;
	$db->executar( $sql );
	$db->commit();
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/inicioContrato");
	die;
}

if( $_POST['ajaxsession'] ){
	$_SESSION['ctrid'] = $_POST['ajaxsession']; 
}

if( $_POST['filtro_entnome']){
	$where .= " AND UPPER(ent.entnome) LIKE UPPER('%{$_POST['filtro_entnome']}%')";
} 
if( $_POST['filtro_tpcid']){
	$where .= " AND ctr.tpcid= '{$_POST['filtro_tpcid']}' ";
} 

if( $_POST['filtro_modid']){
	$where .= " AND ctr.modid= '{$_POST['filtro_modid']}' ";
} 

if( $_POST['filtro_ctrobj']){
	$where .= " AND UPPER (ctr.ctrobj) LIKE UPPER('%{$_POST['filtro_ctrobj']}%')";
} 

if( $_POST['filtro_ctrnum']){
	$where .= " AND ctr.ctrnum = '{$_POST['filtro_ctrnum']}' ";
} 

if( $_POST['filtro_ctrano']){
	$where .= " AND ctr.ctrano = '{$_POST['filtro_ctrano']}' ";
} 

if( $_POST['filtro_sitid']){
	$where .= " AND ctr.sitid= '{$_POST['filtro_sitid']}' ";
} else {
    $where = ' and ctr.sitid = 1 ';
}

if( $_POST['filtro_ctrnummod']){
	$where .= " AND ctr.ctrnummod= '{$_POST['filtro_ctrnummod']}' ";
} 

if( $_POST['filtro_ctrproccontr']){
	$where .= " AND ctr.ctrproccontr LIKE '%{$_POST['filtro_ctrproccontr']}%' ";
} 

if( $_POST['filtro_ctrprocexecctr']){
	$where .= " AND ctr.ctrprocexecctr LIKE '%{$_POST['filtro_ctrprocexecctr']}%' ";
} 

if( $_POST['filtro_ctrprocexecfin']){
	$where .= " AND ctr.ctrprocexecfin LIKE '%{$_POST['filtro_ctrprocexecfin']}%' ";
}

if( $_POST['dtinicio'] && !$_POST['dtfim'] ){
	$where .= " AND ctrdtiniciovig >= '".formata_data_sql($_POST['dtinicio'])."' ";
}
if( $_POST['dtinicio'] && $_POST['dtfim'] ){
	$where .= " AND (ctrdtiniciovig >= '".formata_data_sql($_POST['dtinicio'])."' and ctrdtiniciovig <= '".formata_data_sql($_POST['dtfim'])."' )";
}
if( $_POST['dtinicio2'] && !$_POST['dtfim2'] ){
	$where .= " AND ctrdtfimvig >= '".formata_data_sql($_POST['dtinicio2'])."' ";
}
if( $_POST['dtinicio2'] && $_POST['dtfim2'] ){
	$where .= " AND (ctrdtfimvig >= '".formata_data_sql($_POST['dtinicio2'])."' and ctrdtfimvig <= '".formata_data_sql($_POST['dtfim2'])."' )";
}
if( $_POST['estid'] ){
	$where .= " AND (est.estid = '".$_POST['estid']."' )";
}

 //filtro_valorinicio
if($_POST['filtro_valorinicio'] AND $_POST['filtro_valorfim']){
 	$vl_inicio = str_replace(',','.',str_replace('.','',$_POST['filtro_valorinicio']));
 	$vl_fim = str_replace(',','.',str_replace('.','',$_POST['filtro_valorfim']));
 	$where.=" AND (SELECT CASE WHEN (select 
						sum(tdavlr) as total
				   from contratos.cttermoaditivo adi
				   where 
						adi.tdastatus = 'A'
						and adi.ctrid = ctr.ctrid) IS NULL THEN (
							case 
					   			when ctr.ctrvlrtotal is not null then ctr.ctrvlrtotal
					   			else COALESCE(ctr.ctrvlrinicial,0)
					   		end
		) ELSE (
			(select 
				COALESCE(sum(tdavlr),0) as total
			from contratos.cttermoaditivo adi
			where 
				adi.tdastatus = 'A'
				and adi.ctrid = ctr.ctrid)+(case 
					   			when ctr.ctrvlrtotal is not null then ctr.ctrvlrtotal
					   			else COALESCE(ctr.ctrvlrinicial,0)
					   		end))
		END as vl_total) BETWEEN ".$vl_inicio." AND ".$vl_fim;
}elseif($_POST['filtro_valorinicio'] AND !$_POST['filtro_valorfim']){
	$vl_inicio = str_replace(',','.',str_replace('.','',$_POST['filtro_valorinicio']));
	$where.=" AND (SELECT CASE WHEN (select 
						sum(tdavlr) as total
				   from contratos.cttermoaditivo adi
				   where 
						adi.tdastatus = 'A'
						and adi.ctrid = ctr.ctrid) IS NULL THEN (
							case 
					   			when ctr.ctrvlrtotal is not null then ctr.ctrvlrtotal
					   			else COALESCE(ctr.ctrvlrinicial,0)
					   		end
		) ELSE (
			(select 
				COALESCE(sum(tdavlr),0) as total
			from contratos.cttermoaditivo adi
			where 
				adi.tdastatus = 'A'
				and adi.ctrid = ctr.ctrid)+(case 
					   			when ctr.ctrvlrtotal is not null then ctr.ctrvlrtotal
					   			else COALESCE(ctr.ctrvlrinicial,0)
					   		end))
		END as vl_total) > ".$vl_inicio;
}elseif(!$_POST['filtro_valorinicio'] AND $_POST['filtro_valorfim']){
	$vl_fim = str_replace(',','.',str_replace('.','',$_POST['filtro_valorfim']));
	$where.=" AND (SELECT CASE WHEN (select 
						sum(tdavlr) as total
				   from contratos.cttermoaditivo adi
				   where 
						adi.tdastatus = 'A'
						and adi.ctrid = ctr.ctrid) IS NULL THEN (
							case 
					   			when ctr.ctrvlrtotal is not null then ctr.ctrvlrtotal
					   			else COALESCE(ctr.ctrvlrinicial,0)
					   		end
		) ELSE (
			(select 
				COALESCE(sum(tdavlr),0) as total
			from contratos.cttermoaditivo adi
			where 
				adi.tdastatus = 'A'
				and adi.ctrid = ctr.ctrid)+(case 
					   			when ctr.ctrvlrtotal is not null then ctr.ctrvlrtotal
					   			else COALESCE(ctr.ctrvlrinicial,0)
					   		end))
		END as vl_total) < ".$vl_fim;
}
if( $_POST['pesquisar']){
	$whereOR[] = "removeacento(ent.entnome) ilike removeacento(('%{$_POST['pesquisar']}%'))";
	$whereOR[] = "removeacento(tpc.tpcdsc) ilike removeacento(('%{$_POST['pesquisar']}%'))";
	$whereOR[] = "removeacento(ctr.ctrnum::text) ilike removeacento(('%{$_POST['pesquisar']}%'))";
	$whereOR[] = "removeacento(ctr.ctrnummod) ilike removeacento(('%{$_POST['pesquisar']}%'))";
	$whereOR[] = "removeacento(sit.sitdsc) ilike removeacento(('%{$_POST['pesquisar']}%'))";
	$whereOR[] = "removeacento(ctr.ctrobj) ilike removeacento(('%{$_POST['pesquisar']}%'))";
	$whereOR[] = "removeacento(mod.moddsc) ilike removeacento(('%{$_POST['pesquisar']}%'))";
	$whereOR[] = "to_char(ctr.ctrdtiniciovig,'DD/MM/YYYY') ilike removeacento(('%{$_POST['pesquisar']}%'))";
	$whereOR[] = "to_char(ctr.ctrdtfimvig,'DD/MM/YYYY') ilike removeacento(('%{$_POST['pesquisar']}%'))";
	$whereOR[] = "removeacento(estr.estareasigla) ilike removeacento(('%{$_POST['pesquisar']}%'))";
	$whereOR[] = "removeacento(estr.estarea) ilike removeacento(('%{$_POST['pesquisar']}%'))";
	$whereOR[] = "removeacento(eps.nu_empenho) ilike removeacento(('%" . trim( $_POST['pesquisar'] ) . "%'))";
	$whereOR[] = "removeacento(obf.obfnumero) ilike removeacento(('%" . trim( $_POST['pesquisar'] ) . "%'))";
	$where .= " and (".implode(" or ",$whereOR).")";
} 

$camposSQL = " SELECT ";

if($_REQUEST['consulta'] == 'html'){
	
	$camposSQL .= "
			 DISTINCT
			 	'<center><img align=\"absmiddle\" src=\"/imagens/alterar.gif\"  style=\"cursor: pointer\" onclick=\"javascript: selecionarContrato('|| ctr.ctrid ||' );\" title=\"Selecionar Contrato\">
				".((in_array(PERFIL_GESTOR_FINANCEIRO_UNIDADE, $perfis ) || in_array(PERFIL_CONSULTA_UNIDADE, $perfis )  || in_array(PERFIL_CONSULTA_GERAL, $perfis ))?"":"<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"removerContrato( '|| ctr.ctrid ||' );\" title=\"Excluir Contrato\"/>")."</center>' as ACAO,
		";
} else 
	$camposSQL .= " distinct ctr.ctrid, ";
	$camposSQL .= " tpc.tpcdsc as tipo, ";
	if($_REQUEST['consulta'] == 'html')
		$camposSQL .= " ctr.ctrnum as numerocontrato, ";
	else 
		$camposSQL .= " ctr.ctrnum as numerocontrato, ";
	$camposSQL .= "
   	   ctr.ctrano as ano,
   	   ctr.ctrnummod as numeromodalidade,
	   sit.sitdsc as situacao,
	   to_char(ctr.ctrdtiniciovig,'DD/MM/YYYY') as dtinicio,
	   to_char(ctr.ctrdtfimvig,'DD/MM/YYYY') as dtfim,
	   ctr.ctrdtfimvig - ctr.ctrdtiniciovig as dias,
	";
	if($_REQUEST['consulta'] == 'html')
		$camposSQL .= " '<div align=\"left\"> <a href=\"javascript:selecionarContrato(  ' ||ctr.ctrid || '  );\" title=\"Selecionar Contrato\" > ' ||ctr.ctrobj || ' </div>'  as objeto, ";
	else
		$camposSQL .= " ctr.ctrobj as objeto, ";
	$camposSQL .= "
       ent.entnome as contratada,
       mod.moddsc as modalidade,
		CASE WHEN (select 
						sum(tdavlr) as total
				   from contratos.cttermoaditivo adi
				   where 
						adi.tdastatus = 'A'
						and adi.ctrid = ctr.ctrid) IS NULL THEN (
							case 
					   			when ctr.ctrvlrtotal is not null then ctr.ctrvlrtotal
					   			else COALESCE(ctr.ctrvlrinicial,0)
					   		end
		) ELSE (
			(select 
				COALESCE(sum(tdavlr),0) as total
			from contratos.cttermoaditivo adi
			where 
				adi.tdastatus = 'A'
				and adi.ctrid = ctr.ctrid)+(case 
					   			when ctr.ctrvlrtotal is not null then ctr.ctrvlrtotal
					   			else COALESCE(ctr.ctrvlrinicial,0)
					   		end))
		END as vl_total ";
	
 $sqlAll = " 
 $camposSQL
    FROM
       contratos.ctcontrato AS ctr
       LEFT JOIN contratos.faturacontrato AS fc ON fc.ctrid = ctr.ctrid 
       LEFT JOIN contratos.empenho_siafi AS eps ON eps.epsid = fc.epsid
       LEFT JOIN contratos.ordembancariafatura AS obf ON obf.ftcid = fc.ftcid
       LEFT JOIN entidade.entidade AS ent ON ent.entid = ctr.entidcontratada
       LEFT JOIN contratos.cttipocontrato AS tpc ON tpc.tpcid = ctr.tpcid
       LEFT JOIN contratos.ctanexo anc on anc.ctrid = ctr.ctrid
       LEFT JOIN contratos.ctmodalidadecontrato mod on ctr.modid = mod.modid
       LEFT JOIN contratos.ctsituacaocontrato sit on ctr.sitid = sit.sitid
       LEFT JOIN contratos.contratosetorresponsavel est on ctr.ctrid = est.ctrid
    where ctr.ctrstatus = 'A' 
      $condicao 
     $where 
   ORDER BY ano desc, numerocontrato desc
";

//dbg($sqlAll,d);

if($_REQUEST['consulta'] == 'xls'){
	header('content-type: text/html; charset=ISO-8859-1');
	$cabecalho = array("", "Tipo", "Número", "Ano", "Número Modalidade", "Situação", "Data de Início da Vigência","Data de Término da Vigência","Dias para vencimento" , "Objeto", "Contratada", "Modalidade","Valor");
	$db->sql_to_excel($sqlAll, 'listaContratos', $cabecalho);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' ); 
     
     //dbg($sqlAll,1);
?>
<script src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
	function removerFiltro(){
		document.formulario.filtro.value = "";
		document.formulario.estuf.selectedIndex = 0;
		document.formulario.submit();
	}

	function exibePesquisaAvancada()
	{
		document.getElementById('tbl_pesquisa_normal').style.display = "none";
		document.getElementById('tbl_pesquisa_avancada').style.display = "";
		document.getElementById('hdn_tipo_pesquisa').value = "avancada";
	}
	
	function exibePesquisaSimples()
	{
		document.getElementById('tbl_pesquisa_normal').style.display = "";
		document.getElementById('tbl_pesquisa_avancada').style.display = "none";
		document.getElementById('hdn_tipo_pesquisa').value = "simples";
	}

	function submitenter(myfield,e)
	{		
		var keycode;
		if (window.event) keycode = window.event.keyCode;
		else if (e) keycode = e.which;
		else return true;
	
		if (keycode == '13')
		{
			document.formulario.submit();
			return false;
		}
		else
			return true;
	}
</script>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:none;">
	<tr>
		<td style="padding: 10px;">
			<span
				style="cursor: pointer"
				onclick="cadastrarContrato();"
				title="Novo Contrato"
			>
				<img
					align="absmiddle"
					src="/imagens/gif_inclui.gif"
				/>
				Cadastrar Contrato
			</span>
		</td>
	</tr>
</table>
<?php
		$cabecalho = array("Ação", "Tipo", "Número", "Ano","Número Licitação" , "Situação", "Data de Início da Vigência", "Data de Término da Vigência", "Dias para vencimento", "Objeto", "Contratada", "Modalidade","Valor");

        $rs = $db->carregar($sqlAll);

        function record_sort($records, $field, $reverse=false)
        {
            $hash = array();

            foreach($records as $key => $record)
            {
                $hash[$record[$field].$key] = $record;
            }

            ($reverse)? krsort($hash) : ksort($hash);

            $records = array();

            foreach($hash as $record)
            {
                $records []= $record;
            }

            return $records;
        }
        
        $db->monta_lista_array( $rs, $cabecalho, 25, 10, 'N', '', '','');
        
        for($x = 0; $x < count($rs); $x++) {
            $dtAtual = date("d/m/Y");
            $sql = "SELECT
			DISTINCT
			to_char(tda.tdafimvig::date,'dd/MM/YYYY') AS tdafimvig
			FROM contratos.cttermoaditivo tda
			LEFT JOIN contratos.ctanexo anc on anc.tdaid = tda.tdaid
			LEFT JOIN public.arquivo arq on arq.arqid = anc.arqid
			WHERE tda.tdastatus = 'A' and tda.ctrid = {$rs[$x]['ctrid']}
			order by tdafimvig DESC limit 1";

            if ($rs[$x]['ctrid']) {

                $sqlSuperacao = "SELECT
                  count(restricao.rstsituacao) as qtd, 
                  ctcontrato.ctrid
                FROM
                  contratos.restricao,
                  contratos.ctcontrato
                WHERE
                  ctcontrato.ctrid = restricao.ctrid and
                  rstsituacao = FALSE and
                  ctcontrato.ctrid = {$rs[$x]['ctrid']}
                group by ctcontrato.ctrid";

                $rsSuperacao = $db->carregar($sqlSuperacao);


                if ($rsSuperacao[0]['qtd'] != '') {
                    $rs[$x]['objeto'] = "<a href='http://sig.ebserh.gov.br/contratos/contratos.php?modulo=principal/listaRestricao&acao=A&ctrid={$rsSuperacao[0]['ctrid']}' style='float: left; margin-right: 10px;'><img src='/imagens/restricao2.png' /></a> " . $rs[$x]['objeto'];
                }

                $rsAditivo = $db->carregar($sql);

                if ($rsAditivo) {

                    $dias = dif_data($dtAtual, $rsAditivo[0]['tdafimvig']);
                    //echo $rs[$x]['dtfim'] . " - " . $rsAditivo[0]['tdafimvig'] . " - " . $dias . " | <br>";
                    if ($dias <= 0 || $rs[$x]['situacao'] != 'Vigente') {
                        $dias = 99999;
                    }

                    //if ($rsAditivo[0]['tdafimvig'] > $rs[$x]['dtfim'])
                    //    echo "X ";

                } else {

                    $dias = dif_data($dtAtual, $rs[$x]['dtfim']);
                    if ($dias <= 0 || $rs[$x]['situacao'] != 'Vigente') {
                        $dias = 99999;
                    }
                }

                if ($rsAditivo[0]['tdafimvig'])
                    $rs[$x]['dtfim'] = $rsAditivo[0]['tdafimvig'];
                $rs[$x]['dias'] = $dias;
                unset($rs[$x]['ctrid']);
            }

        // 	Obter uma lista de colunas

        if ($rs) {
            foreach ($rs as $key => $row) {
                $row = preg_replace("/[^a-zA-Z0-9_]/", "", strtr($row['dias'], "áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ", "aaaaeeiooouucAAAAEEIOOOUUC"));
                $crt[$key]  = $row;
            }

            array_multisort($crt, SORT_ASC , $rs);
        }
	}
?>

<script type="text/javascript"><!--
function removerContrato( id ) {
 
	if ( confirm( 'Deseja excluir o Contrato?' ) ) {
		window.location.href = 'contratos.php?modulo=principal/inicioContrato&acao=A&remover='+id;
	}
}

function selecionarContrato( ctrid ) {
	window.location.href = 'contratos.php?modulo=principal/cadContrato&acao=A&ctrid='+ctrid; 
}

function cadastrarContrato(){ 
	var req = new Ajax.Request('?modulo=principal/cadContrato&acao=I', {
				        method:     'post',
				        parameters: '&ajaxunsetsession=1',							         
				        onComplete: function (res) { 
							window.location.href = '?modulo=principal/cadContrato&acao=I';
						}
	});  
}

function enviar_email( cpf ){
	var nome_janela = 'janela_enviar_emai_' + cpf;
	window.open(
		'/geral/envia_email.php?cpf=' + cpf,
		nome_janela,
		'width=650,height=557,scrollbars=yes,scrolling=yes,resizebled=yes'
	);
}
function validaForm(consulta){
	if(consulta == undefined)
		consulta = 'html';
	if(consulta == 'html')
		document.getElementById('consulta').value='html';
	else if(consulta == 'xls')
		document.getElementById('consulta').value='xls';
  	document.formulario.submit(); 
}

function pesquisaByFiltro( tipo ){
	 if(tipo){
	 	window.location.href="?modulo=inicio&acao=C&pesqAvancada=t&tipoPesq="+tipo;
	 }
}

function selecionaContrato( id ){
	var req = new Ajax.Request('contratos.php?modulo=principal/inicioContrato&acao=A', {
					        method:     'post',
					        parameters: '&ajaxsession=' + id,							         
					        onComplete: function (res) {	
								window.location.href = '?modulo=principal/cadContratoAnexo&acao=A';
							}
		});
}

function filtraTipo(estuf)
{
	if( !estuf ){
		return false;
	}
	td 	   = document.getElementById('municipio');
	select = document.getElementsByName('muncod')[0];
	
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('contratos.php?modulo=inicio&acao=C', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,
							        onComplete: function (res)
							        {			 
							        	var inner = 'Município<br/>';
										td.innerHTML = inner+res.responseText;
										td.style.visibility = 'visible';
							        }
							  });
    
}

function exibeInteressado(hspid)
{
	if(hspid == "<?php echo CONTRATANTE_EBSERH ?>"){
		document.getElementById('estid').disabled = false;
	}else{
		document.getElementById('estid').disabled = true;
	}
}

    $( document ).ready(function() {

        $('td[title="Dias para vencimento"]').each(function(td)  {
            value = parseInt($(this).text());
            if (value < 120 && value >= 61) {
                $(this).parent().find('td').css({background: '#FFFFAA' });
            }

            if (value <= 60) {
                $(this).parent().find('td').css({background: '#F2C2B3' });
            }
            
        });
    });
</script>