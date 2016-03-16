<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

$ncoordenadoresresponderam   = $db->pegaUm("SELECT COUNT(*) as n FROM sispacto.gestaomobilizacaoperguntas");
$ncoordenadoresnaoatividades = $db->pegaUm("SELECT COUNT(*) as n FROM sispacto.gestaomobilizacaoperguntas WHERE gmpnaoatividades=true");
$natividadesrealizadas       = $db->pegaUm("SELECT COUNT(*) as n FROM sispacto.gestaomobilizacaocoordenadorlocal WHERE gmcstatus='A'");

$mediacargahoraria           = $db->pegaUm("SELECT AVG(gmccargahoraria) as n FROM sispacto.gestaomobilizacaocoordenadorlocal WHERE gmcstatus='A'");
$mediaparticipantes          = $db->pegaUm("SELECT AVG(gmcqtdparticipantes) as n FROM sispacto.gestaomobilizacaocoordenadorlocal WHERE gmcstatus='A'");


?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td class="SubTituloEsquerda" colspan="2">Atividades</td></tr>
		<tr>
			<td class="SubTituloDireita" width="40%">Número de coordenadores que responderam:</td>
			<td><?=number_format($ncoordenadoresresponderam,0,",",".") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Não realizaram atividades:</td>
			<td><?=number_format($ncoordenadoresnaoatividades,0,",",".") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Quantidade de atividades realizadas:</td>
			<td><?=number_format($natividadesrealizadas,0,",",".") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Carga Horária (Média):</td>
			<td><?=number_format($mediacargahoraria,2,",",".") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Quantidade Participantes (Média):</td>
			<td><?=number_format($mediaparticipantes,2,",",".") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo Atividades:</td>
			<td><?
			$sql = "select CASE WHEN gmcatividade='R' THEN 'Reunião'WHEN gmcatividade='E' THEN 'Encontro'WHEN gmcatividade='S' THEN 'Seminário'WHEN gmcatividade='D' THEN 'Debate'WHEN gmcatividade='M' THEN 'Mesa-redonda'WHEN gmcatividade='C' THEN 'Conversa'WHEN gmcatividade='F' THEN 'Conferência'WHEN gmcatividade='P' THEN 'Painel'WHEN gmcatividade='X' THEN 'Exposição'WHEN gmcatividade='W' THEN 'Workshop'WHEN gmcatividade='I' THEN 'Simpósio'WHEN gmcatividade='G' THEN 'Congresso'WHEN gmcatividade='V' THEN 'Visita'WHEN gmcatividade='O' THEN 'Outra'END as gmcatividade, 
						   count(*) as qtd, 
						   (count(*)*100/(select count(*) from sispacto.gestaomobilizacaocoordenadorlocal where gmcstatus='A')::numeric) as porc 
					from sispacto.gestaomobilizacaocoordenadorlocal 
					where gmcstatus='A' 
					group by gmcatividade";
			
			$cabecalho = array("Atividade","Qtd","%");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%','',true, false, false, true);
			
			?></td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Público Alvo:</td>
			<td><?
			$sql = "select 
					sum(orientadoresdeestudo) as orientadoresdeestudo,
					sum(professoresalfabetizadores) as professoresalfabetizadores,
					sum(diretoresdeescolas) as diretoresdeescolas,
					sum(coordenadorespedagogicos) as coordenadorespedagogicos,
					sum(conselhosescolares) as conselhosescolares,
					sum(conselhodeeducacao) as conselhodeeducacao,
					sum(equipedaseduc) as equipedaseduc,
					sum(outros) as outros,
					sum(universidade) as universidade,
					sum(paisresponsaveis) as paisresponsaveis
					 
					from (
					select  
					CASE WHEN gmcpublicoalvo ilike '%E;%' THEN 1 ELSE 0 END as orientadoresdeestudo,
					CASE WHEN gmcpublicoalvo ilike '%P;%' THEN 1 ELSE 0 END as professoresalfabetizadores,
					CASE WHEN gmcpublicoalvo ilike '%D;%' THEN 1 ELSE 0 END as diretoresdeescolas,
					CASE WHEN gmcpublicoalvo ilike '%C;%' THEN 1 ELSE 0 END as coordenadorespedagogicos,
					CASE WHEN gmcpublicoalvo ilike '%M;%' THEN 1 ELSE 0 END as conselhosescolares,
					CASE WHEN gmcpublicoalvo ilike '%N;%' THEN 1 ELSE 0 END as conselhodeeducacao,
					CASE WHEN gmcpublicoalvo ilike '%S;%' THEN 1 ELSE 0 END as equipedaseduc,
					CASE WHEN gmcpublicoalvo ilike '%O;%' THEN 1 ELSE 0 END as outros,
					CASE WHEN gmcpublicoalvo ilike '%U;%' THEN 1 ELSE 0 END as universidade,
					CASE WHEN gmcpublicoalvo ilike '%R;%' THEN 1 ELSE 0 END as paisresponsaveis
					from sispacto.gestaomobilizacaocoordenadorlocal 
					where gmcstatus='A' 
					) foo";
			
			$publicoalvo = $db->pegaLinha($sql);
			
			$arrPublicoAlvo = array(0 => array('descricao' => 'Orientadores de Estudo', 'qtd' => $publicoalvo['orientadoresdeestudo'], 'porc' => round(($publicoalvo['orientadoresdeestudo']/$natividadesrealizadas)*100,2)),
									1 => array('descricao' => 'Professores Alfabetizadores', 'qtd' => $publicoalvo['professoresalfabetizadores'], 'porc' => round(($publicoalvo['professoresalfabetizadores']/$natividadesrealizadas)*100,2)),
									2 => array('descricao' => 'Diretores de Escolas', 'qtd' => $publicoalvo['diretoresdeescolas'], 'porc' => round(($publicoalvo['diretoresdeescolas']/$natividadesrealizadas)*100,2)),
									3 => array('descricao' => 'Coordenadores Pedagógicos', 'qtd' => $publicoalvo['coordenadorespedagogicos'], 'porc' => round(($publicoalvo['coordenadorespedagogicos']/$natividadesrealizadas)*100,2)),
									4 => array('descricao' => 'Conselhos Escolares', 'qtd' => $publicoalvo['conselhosescolares'], 'porc' => round(($publicoalvo['conselhosescolares']/$natividadesrealizadas)*100,2)),
									5 => array('descricao' => 'Conselho de Educação', 'qtd' => $publicoalvo['conselhodeeducacao'], 'porc' => round(($publicoalvo['conselhodeeducacao']/$natividadesrealizadas)*100,2)),
									6 => array('descricao' => 'Equipe da SEDUC', 'qtd' => $publicoalvo['equipedaseduc'], 'porc' => round(($publicoalvo['equipedaseduc']/$natividadesrealizadas)*100,2)),
									7 => array('descricao' => 'Outros', 'qtd' => $publicoalvo['outros'], 'porc' => round(($publicoalvo['outros']/$natividadesrealizadas)*100,2)),
									8 => array('descricao' => 'Universidade', 'qtd' => $publicoalvo['universidade'], 'porc' => round(($publicoalvo['universidade']/$natividadesrealizadas)*100,2)),
									9 => array('descricao' => 'Pais ou responsáveis', 'qtd' => $publicoalvo['paisresponsaveis'], 'porc' => round(($publicoalvo['paisresponsaveis']/$natividadesrealizadas)*100,2))
									);
			
			$cabecalho = array("Público Alvo","Qtd","%");
			$db->monta_lista_simples($arrPublicoAlvo,$cabecalho,1000,5,'N','100%','',true, false, false, true);
			
			?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">A Secretaria de Educação está assegurando as condições de deslocamento, alimentação e hospedagem para participação nos encontros presenciais dos Orientadores de Estudo e dos Professores Alfabetizadores, quando necessário?</td>
			<td><?
			$sql = "select 
					CASE WHEN gmppergunta1='S' THEN 'Sempre' WHEN gmppergunta1='A' THEN 'Às vezes' WHEN gmppergunta1='N' THEN 'Nunca' END as re, 
					count(*) as qtd,  
					(count(*)*100/(select count(*) from sispacto.gestaomobilizacaoperguntas)::numeric) as porc
					from sispacto.gestaomobilizacaoperguntas group by gmppergunta1 order by 2 desc";
			
			$cabecalho = array("Resposta","Qtd","%");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%','',true, false, false, false);
			
			?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">A Secretaria de Educação participou de alguma reunião convocada pela Coordenação Institucional do Pacto no seu estado?</td>
			<td><?
			$sql = "select 
					CASE WHEN gmppergunta3='1' THEN 'Sim, participamos de mais de uma reunião' WHEN gmppergunta3='2' THEN 'Sim, participamos de uma reunião' WHEN gmppergunta3='3' THEN 'Não participamos de nenhuma reunião' ELSE 'Não identificado' END as re, 
					count(*) as qtd,  
					(count(*)*100/(select count(*) from sispacto.gestaomobilizacaoperguntas)::numeric) as porc
					from sispacto.gestaomobilizacaoperguntas group by gmppergunta3 order by 2 desc";
			
			$cabecalho = array("Resposta","Qtd","%");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%','',true, false, false, false);
			
			?></td>
		</tr>
		
		
	</table>