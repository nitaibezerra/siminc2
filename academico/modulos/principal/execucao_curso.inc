<?php

if ( $_REQUEST["requisicaolista"] == "editais" ){
	
	$sql = "SELECT 
				'<center>
					<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"curso_atualizaedital(' || edpid || ');\">
					<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"curso_excluiedital(' || edpid || ');\">
				 </center>' as acao,
				edpnumero,
				to_char(edpdtcriacao, 'DD/MM/YYYY'), 
				edpnumvagas,
				usunome 
			FROM
				academico.editalcurso ae
			INNER JOIN
				seguranca.usuario su ON su.usucpf = ae.usucpf
			INNER JOIN
				academico.execucao ex ON ex.exeid = ae.exeid
			WHERE
				edpano = '{$_REQUEST["anobase"]}' AND
				ex.curid = '{$_REQUEST["curso"]}' AND 
				edpstatus = 'A'";
	
	$cabecalho = array( "Ação", "Nº Edital", "Data do Edital", "Nº de vagas", "Inserido Por");
	$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '90%');
	die;
	
}

// objetos do módulo
$curso = new cursos();
$autoriazacaoconcursos = new autoriazacaoconcursos();

if ( $_REQUEST["excluiedital"] == 1 ){
	$curso->excluiEdital($_REQUEST["edpid"]);
}

// variáveis utilizadas na tela
$curid = $_SESSION["academico"]["curid"];
$_SESSION['academico']['entidcampus'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entidcampus'];  
$entidcampus = $_SESSION['academico']['entidcampus'] ? $_SESSION['academico']['entidcampus'] : $_REQUEST['entid']; 
$_SESSION['academico']['entid'] = $autoriazacaoconcursos->buscaentidade($entidcampus);


require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Execução do Curso", "");


// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);

// cria o cabeçalho padrão do módulo
echo $autoriazacaoconcursos->cabecalho_entidade($entidcampus, '', $curid);

?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script>
	
	function curso_vereditais( id, acao, anobase, curso ){
		
		coluna     = document.getElementById('tr_editais_' + id);
		colunamais = document.getElementById('acao_' + id);
		td 		   = document.getElementById('td_'+id);
		 
		var url = 'academico.php?modulo=principal/execucao_curso&acao=A';
		var parametros = "&requisicaolista=editais&anobase=" + anobase + "&curso=" + curso;
	
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(resp) {
					if (acao == 'ver'){
						if (document.selection){
							coluna.style.display = 'block';
						}else{
							coluna.style.display = 'table-row';
						}
						td.innerHTML = resp.responseText;
						colunamais.innerHTML = "<img src='../imagens/menos.gif' style='cursor:pointer;' onclick='curso_vereditais(" + id + ", \"fechar\", " + anobase + ", " + curso + ");'/>";
					}else{
						coluna.style.display = 'none';
						colunamais.innerHTML = "<img src='../imagens/mais.gif' style='cursor:pointer;' onclick='curso_vereditais(" + id + ", \"ver\", " + anobase + ", " + curso + ");'/>";
						td.innerHTML = '';
					}
				}
			}
		);
				
	}
	
	function curso_atualizaedital( edpid ){
		window.open('academico.php?modulo=principal/editalcurso&acao=A&edpid=' + edpid,'inserir edital','scrollbars=yes,height=540,width=630');
	}
	
	function curso_excluiedital( edpid ){
		if(confirm("Deseja realmente excluir o edital?")){
			window.location = 'academico.php?modulo=principal/execucao_curso&acao=A&excluiedital=1&edpid=' + edpid;
		}
	}
	
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
	<tr>
		<td class="subtitulocentro"> Detalhamento </td>
	</tr>
	<tr>
		<td>
			
			<?php
			
				$sql = "SELECT DISTINCT
							turdsc as turno,
							exeanobase as inicio,
							(SELECT coalesce(sum(edpnumvagas),0) FROM academico.editalcurso WHERE edpano = ec.edpano AND edpstatus = 'A' AND exeid = ae.exeid) as vagasvestibular,
							coalesce(pifingressantes, 0) as ingresso,
							coalesce(pifmatriculados, 0) as matricula,
							coalesce(pifconcluintes, 0) as concluintes,
							coalesce(pifvagasvest, 0) as vagas,
							ae.curid as curso
						FROM 
							academico.editalcurso ec
						INNER JOIN
							academico.execucao ae ON ae.exeid = ec.exeid
						INNER JOIN
							academico.curso ac ON ac.curid = ae.curid
						INNER JOIN
							academico.turno at ON at.turid = ac.turidprevisto
						LEFT JOIN 
							academico.pingifes ap ON ap.curid = ac.curid
						WHERE
							ae.curid = {$curid} AND
							edpstatus = 'A'";
				
				$dados_edital = $db->carregar($sql);

				if ( $dados_edital ){
			?>
					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
						<tr>
							<td colspan="5" bgcolor="cccccc"></td>
							<td colspan="4" align="center" bgcolor="feff7f"><b>Fonte: PINGIFES</b></td>
						</tr>
						<tr align="center">
							<td bgcolor="cccccc"><b>Editais</b></td>
							<td bgcolor="cccccc"><b>Turno</b></td>
							<td bgcolor="cccccc"><b>Início</b></td>
							<td bgcolor="cccccc"><b>Vagas Projetadas</b></td>
							<td bgcolor="cccccc"><b>Vagas Vestibular</b></td>
							<td bgcolor="feff7f"><b>Vagas</b></td>
							<td bgcolor="feff7f"><b>Ingresso</b></td>
							<td bgcolor="feff7f"><b>Matriculas</b></td>
							<td bgcolor="feff7f"><b>Concluintes</b></td>
						</tr>
						<?php 
	
							for( $i = 0; $i < count($dados_edital); $i++ ){
								
								print "<tr align='center'>"
									. "		<td bgcolor='ececec' id='acao_{$i}'><img src='../imagens/mais.gif' style='cursor:pointer;' onclick='curso_vereditais({$i}, \"ver\", {$dados_edital[$i]["inicio"]}, {$dados_edital[$i]["curso"]});'/></td>" 
									. "		<td bgcolor='ececec'> {$dados_edital[$i]["turno"]} </td>"
									. "		<td bgcolor='ececec'> {$dados_edital[$i]["inicio"]} </td>"
									. "		<td bgcolor='ececec'> 0 </td>"
									. "		<td bgcolor='ececec'> {$dados_edital[$i]["vagasvestibular"]} </td>"
									. "		<td bgcolor='feff7f'> {$dados_edital[$i]["vagas"]} </td>"
									. "		<td bgcolor='feff7f'> {$dados_edital[$i]["ingresso"]} </td>"
									. "		<td bgcolor='feff7f'> {$dados_edital[$i]["matricula"]} </td>"
									. "		<td bgcolor='feff7f'> {$dados_edital[$i]["concluintes"]} </td>"
									. "</tr>"
									. "<tr id='tr_editais_{$i}' style='display:none;'><td id='td_{$i}' colspan='9'></td></tr>";
									
							}
									
						?>
					</table>
				
			<?php }else{ ?>
				
					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
						<tr><td align="center" style="color:#cc0000;">Não existem editais cadastrados para este curso.</td></tr>
					</table>
					
			<?php } ?>
			
		</td>
	</tr>
	<tr bgcolor="#cccccc">
		<td colspan="2">
			<input type="button" value="Inserir Edital" onclick="window.open('academico.php?modulo=principal/editalcurso&acao=A','inserir edital','scrollbars=yes,height=540,width=630');" style="cursor: pointer;"/>
			<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
		</td>
	</tr>
</table>