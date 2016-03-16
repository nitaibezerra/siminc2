<?php
require_once APPRAIZ . "includes/cabecalho.inc";
require APPRAIZ . "minc/modulos/principal/monitoramento/monitoramento2Controller.php";
$perfis = pegaPerfilGeral();

$habilita = 'S';
$disable = "";

if(in_array(PERFIL_MINC_CONSULTA, $perfis) ){
	$habilita = 'N';
	$disable ='disabled="disabled"';
}
$arMnuid = array();

if ($docid) {
    $sqlDocumento = "select * from workflow.documento where docid = {$docid}";
    $resultDocumento = $db->pegaLinha($sqlDocumento);

    if ($resultDocumento && !in_array($resultDocumento[esdid], array(/*ESTADO_DOCUMENTO_AVALIACAO, ESTADO_DOCUMENTO_FINALIZADO, ESTADO_DOCUMENTO_AVALIADO, */ESTADO_DOCUMENTO_ENVIADO_FNDE/*, ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR, ESTADO_DOCUMENTO_AVALIACAO*/))) {
        $arMnuid = array(MNUID_AVALIACAO, MNUID_MONITORAMENTO);
    }
}

$db->cria_aba($abacod_tela, $url, '', $arMnuid);

$titulo = "Mais Cultura nas Escolas";
$subtitulo = "Monitoramento";

echo monta_titulo($titulo, $subtitulo);
echo cabecalho();

require APPRAIZ . "minc/modulos/principal/monitoramento/monitoramento2_js.php";
?>
	<form id="form" enctype="multipart/form-data" name="form" method="POST">
		<input type="hidden" value="" id="requisicao" name="requisicao">
		<table align="center" width="95%" border="0" cellpadding="2" cellspacing="1">
			<tr bgcolor="#e7e7e7">
				<td align="center" style="font-size: 16px" colspan="2">
					<br/>

					<p><b>ATENÇÃO! Novas perguntas foram agregadas ao Monitoramento, no intuito de aprofundar as informações acerca do desenvolvimento do Programa Mais Cultura nas Escolas. 
							Contamos com a colaboração de todos/as!</b></p>
					<br/>
				</td>
			</tr>
			<br/>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "2">
					<p><b>1.COM QUAIS EIXOS TEMÁTICOS O PLANO DE ATIVIDAE CULTURAL ESTÁ DIALOGANDO EFETIVAMENTE?</b></p>
				</td>
			</tr>
			<tr>
				<td colspan = "2">
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td width="50%">
								<p><input type="checkbox" class="questao1" name="oprid[1]"  value="t"   <?=(($oprid[1]=="t")?"checked":"") ?> <?=$disable?> > 
									Criação circulação e difusão da produção artística</p>
							</td>
							<td width="50%">
								<p><input type="checkbox" class="questao1" name="oprid[6]"  value="t"   <?=(($oprid[6]=="t")?"checked":"")  ?> <?=$disable?> > 
								Educação patrimonial - patrimônio material e imaterial, memória, identidade e vínculo social</p>
							</td>
						</tr>
							<td>
								<p><input type="checkbox" class="questao1" name="oprid[2]"  value="t"  <?=(($oprid[2]=="t")?"checked":"")  ?> <?=$disable?>> 
								Cultura afro-brasileira</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao1" name="oprid[7]"  value="t"  <?=(($oprid[7]=="t")?"checked":"")  ?> <?=$disable?>> 
								Promoção cultural e pedagógica em espaços culturais</p>
							</td>
						<tr>
							<td>
								<p><input type="checkbox" class="questao1" name="oprid[3]"  value="t"  <?=(($oprid[3]=="t")?"checked":"")  ?> <?=$disable?>> 
								Cultura digital e comunicação</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao1" name="oprid[8]"  value="t"  <?=(($oprid[8]=="t")?"checked":"")  ?> <?=$disable?>> 
								Residência de artistas para pesquisa e experimentação nas escolas</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class= "questao1" name="oprid[4]"  value="t"  <?=(($oprid[4]=="t")?"checked":"")  ?> <?=$disable?>> 
								Culturas indígenas</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao1" name="oprid[9]"  value="t"  <?=(($oprid[9]=="t")?"checked":"")  ?> <?=$disable?>>
								Tradição oral</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao1" name="oprid[5]"  value="t"  <?=(($oprid[5]=="t")?"checked":"")  ?> <?=$disable?>> 
								Educação Museal</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao1" name="oprid[10]"  value="t"  <?=(($oprid[10]=="t")?"checked":"")  ?> <?=$disable?>> 
								Formação literária e difusão da cultura</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "3">
					<p><b>
						2.O Programa Mais Cultura possibilitou o envolvimento de docentes da escola no planejamento / desenvolvimento das atividades?
					</b></p>
				</td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda">
					<p><input type="radio" class="envolvimentodocentes" name="envolvimentodocentes_2"  value="1" <?=($queid[3]['bol'] == 't'?"checked":"")  ?> <?=$disable?>>Sim
					<input type="radio" class="envolvimentodocentes" name="envolvimentodocentes_2"  value="2"   <?=($queid[2]=='f'?"checked":"")  ?> <?=$disable?>>Não</p>
				</td>
			</tr>
			<tr id="tr_envolvimentodocentesqtd" <?= (($queid[3]['bol'] == 't') ? "" : "style=\"display:none\"") ?> >
				<td>
					Quantos Docentes?
            		<?php echo campo_texto('qtddocentes', 'N', $habilita, '', 10, 8, "####",'' , '', '', 0, 'id="qtddocentes"','' , $queid[3]['respostageral']); ?>
				</td>
			</tr>
			<tr id="tr_envolvimentodocentes" <?= (($queid[3]['bol'] == "t") ? "" : "style=\"display:none\"") ?>>
				<td>
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td >
								<p><input type="checkbox" class="questao2" name="oprid[11]"  value="t"   <?=(($oprid[11]=="t")?"checked":"")  ?> <?=$disable?>> 
									História</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao2" name="oprid[14]"  value="t"   <?=(($oprid[14]=="t")?"checked":"")  ?> <?=$disable?>> 
									Ciências
								</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao2" name="oprid[17]"  value="t"   <?=(($oprid[17]=="t")?"checked":"")  ?> <?=$disable?>> 
								Literatura</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao2" name="oprid[20]"  value="t"   <?=(($oprid[20]=="t")?"checked":"")  ?> <?=$disable?>> 
								Biologia</p>
							</td>
						</tr>
							<td>
								<p><input type="checkbox" class="questao2" name="oprid[12]"  value="t"  <?=(($oprid[12]=="t")?"checked":"")  ?> <?=$disable?>> 
								Geografia</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao2" name="oprid[15]"  value="t"  <?=(($oprid[15]=="t")?"checked":"")  ?> <?=$disable?>> 
								Artes</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao2" name="oprid[18]"  value="t"   <?=(($oprid[18]=="t")?"checked":"")  ?> <?=$disable?>> 
								Educação Física</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao2" name="oprid[11]"  value="t"   <?=(($oprid[11]=="t")?"checked":"")  ?> <?=$disable?>> 
								Quimica</p>
							</td>
						<tr>
							<td>
								<p><input type="checkbox" class="questao2" name="oprid[13]"  value="t"  <?=(($oprid[13]=="t")?"checked":"")  ?> <?=$disable?>> 
									Matemática
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao2" name="oprid[16]"  value="t"  <?=(($oprid[16]=="t")?"checked":"")  ?> <?=$disable?>> 
								Português</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao2" name="oprid[19]"  value="t"   <?=(($oprid[19]=="t")?"checked":"")  ?> <?=$disable?>> 
								Física</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "2">
					<p><b>
						3.O Plano de Atividade Cultural dialoga com quais temas e políticas:
					</b></p>
				</td>
			</tr>
			<tr>
				<td colspan = "2">
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td>
								<p><input type="checkbox" class="questao3" name="oprid[22]"  value="t"   <?=(($oprid[22]=="t")?"checked":"")  ?> <?=$disable?>> 
								Combate à violência.</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao3" name="oprid[26]"  value="t"   <?=(($oprid[26]=="t")?"checked":"")  ?> <?=$disable?>> 
								Étnico Racial.</p>
							</td>
						</tr>
							<td>
								<p><input type="checkbox" class="questao3" name="oprid[23]"  value="t"  <?=(($oprid[23]=="t")?"checked":"")  ?> <?=$disable?>> 
								Educação Ambiental.</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao3" name="oprid[27]"  value="t"  <?=(($oprid[27]=="t")?"checked":"")  ?> <?=$disable?>> 
								Gênero.</p>
							</td>
						<tr>
							<td>
								<p><input type="checkbox" class="questao3" name="oprid[24]"  value="t"  <?=(($oprid[24]=="t")?"checked":"")  ?> <?=$disable?>> 
								Educação Inclusiva.</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao3" name="oprid[28]"  value="t"  <?=(($oprid[28]=="t")?"checked":"")  ?> <?=$disable?>> 
								LGBTT(Lésbicas, Gays, Bissexuais, Transexuais e Travestis).</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao3" name="oprid[25]"  value="t"  <?=(($oprid[25]=="t")?"checked":"")  ?> <?=$disable?>> 
								Educação Popular.
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao3" name="oprid[29]"  value="t"  <?=(($oprid[29]=="t")?"checked":"")  ?> <?=$disable?>>
								Nenhuma das anteriores.</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "2">
					<p><b>
						4.O Projeto Político Pedagógico da escola contempla alguma das temáticas abaixo?
					</b></p>
				</td>
			</tr>
			<tr>
				<td colspan = "2">
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td >
								<p><input type="checkbox" class="questao4" name="oprid[30]"  value="t"   <?=(($oprid[30]=="t")?"checked":"")  ?> <?=$disable?>> 
								História e cultura afro-brasileira;
								</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao4" name="oprid[34]"  value="t"   <?=(($oprid[34]=="t")?"checked":"")  ?> <?=$disable?>> 
								Diversidade étnica, de gênero e religiosa;
								</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao4" name="oprid[38]"  value="t"   <?=(($oprid[38]=="t")?"checked":"")  ?> <?=$disable?>> 
								Educação Inclusiva;
								</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao4" name="oprid[42]  value="t"   <?=(($oprid[42]=="t")?"checked":"")  ?> <?=$disable?>> 
								Nenhuma das anteriores
								</p>
							</td>
						</tr>
							<td>
								<p><input type="checkbox" class="questao4" name="oprid[31]"  value="t"  <?=(($oprid[31]=="t")?"checked":"")  ?> <?=$disable?>> 
								História e culturas indígenas;</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao4" name="oprid[35]"  value="t"  <?=(($oprid[35]=="t")?"checked":"")  ?> <?=$disable?>> 
								Democracia e protagonismo dos estudantes;
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao4" name="oprid[39]"  value="t"  <?=(($oprid[39]=="t")?"checked":"")  ?> <?=$disable?>> 
								Educação Ambiental;
								</p>
							</td>
						<tr>
							<td>
								<p><input type="checkbox" class="questao4" name="oprid[32]"  value="t"  <?=(($oprid[32]=="t")?"checked":"")  ?> <?=$disable?>> 
								Música e Educação musical;</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao4" name="oprid[36]"  value="t"  <?=(($oprid[36]=="t")?"checked":"")  ?> <?=$disable?>> 
								Território educativo;</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao4" name="oprid[40]"  value="t"  <?=(($oprid[40]=="t")?"checked":"")  ?> <?=$disable?>> 
								Direitos Humanos;</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao4" name="oprid[33]"  value="t"  <?=(($oprid[33]=="t")?"checked":"")  ?> <?=$disable?>> 
								Participação da comunidade;
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao4" name="oprid[37]"  value="t"  <?=(($oprid[37]=="t")?"checked":"")  ?> <?=$disable?>>
								Reconhecimento e fortalecimento da diversidade cultura local;</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao4" name="oprid[41]"  value="t"  <?=(($oprid[41]=="t")?"checked":"")  ?> <?=$disable?>>
								Educação Popular;</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "2">
					<p><b>
						5.Assinale quais espaços foram / estão sendo frequentados nas atividades do Programa Mais Cultura nas Escolas:
					</b></p>
				</td>
			</tr>
			<tr>
				<td colspan = "2">
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td >
								<p><input type="checkbox" class="questao5" name="oprid[43]"  value="t"   <?=(($oprid[43]=="t")?"checked":"")  ?> <?=$disable?>> 
								Aldeia Indígena
								</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao5" name="oprid[53]"  value="t"   <?=(($oprid[53]=="t")?"checked":"")  ?> <?=$disable?>> 
								Cinema
								</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao5" name="oprid[63]"  value="t"   <?=(($oprid[63]=="t")?"checked":"")  ?> <?=$disable?>> 
								Praças Públicas
								</p>
							</td>
						</tr>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[44]"  value="t"  <?=(($oprid[44]=="t")?"checked":"")  ?> <?=$disable?>> 
								Assentamento da Reforma Agrária</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[54]"  value="t"  <?=(($oprid[54]=="t")?"checked":"")  ?> <?=$disable?>> 
								Cineclube
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[64]"  value="t"  <?=(($oprid[64]=="t")?"checked":"")  ?> <?=$disable?>> 
								Parques
								</p>
							</td>
						<tr>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[45]"  value="t"  <?=(($oprid[45]=="t")?"checked":"")  ?> <?=$disable?>> 
								Acampamento da Reforma Agrária</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[55]"  value="t"  <?=(($oprid[55]=="t")?"checked":"")  ?> <?=$disable?>> 
								Circo</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[65]"  value="t"  <?=(($oprid[65]=="t")?"checked":"")  ?> <?=$disable?>> 
								Quadra(s)/ Ginásio(s) locais</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[46]"  value="t"  <?=(($oprid[46]=="t")?"checked":"")  ?> <?=$disable?>> 
								Associação(es) Comunitária(s) 
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[56]"  value="t"  <?=(($oprid[56]=="t")?"checked":"")  ?> <?=$disable?>>
								Comunidade Cigana</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[66]"  value="t"  <?=(($oprid[66]=="t")?"checked":"")  ?> <?=$disable?>>
								Teatro</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[47]"  value="t"  <?=(($oprid[47]=="t")?"checked":"")  ?> <?=$disable?>> 
								Anfiteatro / Auditório
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[57]"  value="t"  <?=(($oprid[57]=="t")?"checked":"")  ?> <?=$disable?>>
								Escolas de Artes (dança, música, teatro, plásticas, etc)
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[67]"  value="t"  <?=(($oprid[67]=="t")?"checked":"")  ?> <?=$disable?>>
								Instituição Religiosa</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[48]"  value="t"  <?=(($oprid[48]=="t")?"checked":"")  ?> <?=$disable?>> 
								Biblioteca(s)
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[58]"  value="t"  <?=(($oprid[58]=="t")?"checked":"")  ?> <?=$disable?>>
								Escola de Samba</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[68]"  value="t"  <?=(($oprid[68]=="t")?"checked":"")  ?> <?=$disable?>>
								Quilombo</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[49]"  value="t"  <?=(($oprid[49]=="t")?"checked":"")  ?> <?=$disable?>> 
								Biblioteca(s) Comunitária(s)
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[59]"  value="t"  <?=(($oprid[59]=="t")?"checked":"")  ?> <?=$disable?>>
								Museu</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[69]"  value="t"  <?=(($oprid[69]=="t")?"checked":"")  ?> <?=$disable?>>
								Outros.
								</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[50]"  value="t"  <?=(($oprid[50]=="t")?"checked":"")  ?> <?=$disable?>> 
								Biblioteca da escola
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[60]"  value="t"  <?=(($oprid[60]=="t")?"checked":"")  ?> <?=$disable?>>
								Ponto de Cultura</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[51]"  value="t"  <?=(($oprid[51]=="t")?"checked":"")  ?> <?=$disable?>> 
								Centro(s) Cultural(is)
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[61]"  value="t"  <?=(($oprid[61]=="t")?"checked":"")  ?> <?=$disable?>>
								Ponto de Memória</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[52]"  value="t"  <?=(($oprid[52]=="t")?"checked":"")  ?> <?=$disable?>> 
								CEU (Centro de Artes e Esportes Unificados)
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao5" name="oprid[62]"  value="t"  <?=(($oprid[62]=="t")?"checked":"")  ?> <?=$disable?>>
								Ponto de Leitura</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "2">
					<p><b>
						6.As experiências culturais e artísticas vivenciadas no Mais Cultura nas Escolas proporcionaram: 
					</b></p>
				</td>
			</tr>
			<tr>
				<td colspan = "2">
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td >
								<p><input type="checkbox" class="questao6" name="oprid[70]"  value="t"   <?=(($oprid[70]=="t")?"checked":"")  ?> <?=$disable?>> 
								Práticas pedagógicas não formais;
								</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao6" name="oprid[80]"  value="t"   <?=(($oprid[80]=="t")?"checked":"")  ?> <?=$disable?>> 
								Apreciação de atividades artísticas e culturais fora da escola (espetáculos, mostras, festivais, apresentações, saraus, etc.);
								</p>
							</td>
						</tr>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[71]"  value="t"  <?=(($oprid[71]=="t")?"checked":"")  ?> <?=$disable?>> 
								Estudos de meio, trabalhos e/ou pesquisas de campo;</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[81]"  value="t"  <?=(($oprid[81]=="t")?"checked":"")  ?> <?=$disable?>> 
								Reconhecimento do saber popular
								</p>
							</td>
						<tr>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[72]"  value="t"  <?=(($oprid[72]=="t")?"checked":"")  ?> <?=$disable?>> 
								Realização de pesquisa e/ou mapeamento do patrimônio cultural material e imaterial;
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[82]"  value="t"  <?=(($oprid[82]=="t")?"checked":"")  ?> <?=$disable?>> 
								Reconhecimento da história e cultura local</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[73]"  value="t"  <?=(($oprid[73]=="t")?"checked":"")  ?> <?=$disable?>> 
								Aprendizado ou aperfeiçoamento de uma linguagem artística; 
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[83]"  value="t"  <?=(($oprid[83]=="t")?"checked":"")  ?> <?=$disable?>>
								Reflexão sobre a realidade local (social, política e cultural)</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[74]"  value="t"  <?=(($oprid[74]=="t")?"checked":"")  ?> <?=$disable?>> 
								Produção artística e cultural autoral dos participantes do Programa Mais Cultura nas Escolas;
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[84]"  value="t"  <?=(($oprid[84]=="t")?"checked":"")  ?> <?=$disable?>>
								Apreciação da produção artística e cultural da comunidade local;
								</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[75]"  value="t"  <?=(($oprid[75]=="t")?"checked":"")  ?> <?=$disable?>> 
								Apresentação da produção artística e cultural dos participantes do Programa Mais Cultura nas Escolas, dentro do espaço escolar;
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[85]"  value="t"  <?=(($oprid[85]=="t")?"checked":"")  ?> <?=$disable?>>
								Entusiasmo e envolvimento da comunidade escolar nos processos educativos;
								</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[76]"  value="t"  <?=(($oprid[76]=="t")?"checked":"")  ?> <?=$disable?>> 
								Apresentação da produção artística e cultural dos participantes do Programa Mais Cultura nas Escolas, fora do espaço escolar;
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[86]"  value="t"  <?=(($oprid[86]=="t")?"checked":"")  ?> <?=$disable?>>
								Ações de sustentabilidade</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[77]"  value="t"  <?=(($oprid[77]=="t")?"checked":"")  ?> <?=$disable?>> 
								Apresentações, espetáculos ou mostras no espaço escolar com grupos culturais ou artistas convidados;
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[87]"  value="t"  <?=(($oprid[87]=="t")?"checked":"")  ?> <?=$disable?>>
								Ações de preservação do meio ambiente
								</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[78]"  value="t"  <?=(($oprid[78]=="t")?"checked":"")  ?> <?=$disable?>> 
								Exploração de outros espaços educativos além da escola;
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[88]"  value="t"  <?=(($oprid[88]=="t")?"checked":"")  ?> <?=$disable?>>
								Nenhuma das anteriores.
								</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao6" name="oprid[79]"  value="t"  <?=(($oprid[79]=="t")?"checked":"")  ?> <?=$disable?>> 
								Visitas a equipamentos culturais como teatros, cinemas, centros culturais, museus e etc.;
								</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "3">
					<p><b>
					7. As atividades colaboraram para a integração do saber popular e do saber formal?
					</b></p>
				</td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda">
					<p><input type="radio" class="colaboraintegracao" name="colaboraintegracao_7"  value="1"  <?=($queid[10]=='t'?"checked":"")  ?><?=$disable?>>Sim
					<input type="radio" class="colaboraintegracao" name="colaboraintegracao_7"  value="2"   <?=($queid[9]=='f'?"checked":"")  ?><?=$disable?>>Não</p>
				</td>
			</tr>
			<tr id="tr_colaboraintegracao" <?= (($queid[10] == 't') ? "" : "style=\"display:none\"") ?>>
				<td>
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td>
								Selecione pelo menos um aspecto que contribuiu para esta integração:
							</td>
						</tr>
						<tr>
							<td >
								<p><input type="checkbox" class="questao7" name="oprid[89]"  value="t"   <?=(($oprid[89]=="t")?"checked":"")  ?><?=$disable?> > 
								Diálogo entre comunidade escolar e saberes e fazeres da comunidade em que a escola está inserida.
								</p>
							</td>
						</tr>
							<td>
								<p><input type="checkbox" class="questao7" name="oprid[90]"  value="t"  <?=(($oprid[90]=="t")?"checked":"")  ?> <?=$disable?>> 
								Reconhecimento dos membros da comunidade atuantes em manifestações da cultura popular ou tradicional.</p>
							</td>
						<tr>
							<td>
								<p><input type="checkbox" class="questao7" name="oprid[91]"  value="t"  <?=(($oprid[91]=="t")?"checked":"")  ?> <?=$disable?>> 
									Diálogo entre docentes e mestres e/ou artistas da cultura popular, cultura afro brasileira e/ou cultura indígena.
								</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao7" name="oprid[92]"  value="t"  <?=(($oprid[92]=="t")?"checked":"")  ?><?=$disable?>> 
								Estudantes em contato com saberes e fazeres de mestres da cultura popular, cultura afro brasileira e/ou cultura indígena.
								</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "3">
					<p><b>
					8. As práticas artísticas e culturais desenvolvidas no Plano de Atividade Cultural contribuíram para melhorar os processos de alfabetização e leitura dos estudantes?
					</b></p>
				</td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda">
					<p><input type="radio" class="praticasculturais" name="praticasculturais_8"  value="1" <?=(($queid[12]=='t')?"checked":"")  ?><?=$disable?>>Sim
					<input type="radio" class="praticasculturais" name="praticasculturais_8"  value="2"  <?=(($queid[11]=='f')?"checked":"")  ?><?=$disable?>>Não</p>
				</td>
			</tr>
			<tr id="tr_praticasculturais" <?= (($queid[12] == "t") ? "" : "style=\"display:none\"") ?>>
				<td>
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td>
							Selecione, pelo menos, um aspecto que contribuiu para este processo:
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao8" name="oprid[93]"  value="t"   <?=(($oprid[93]=="t")?"checked":"")  ?> <?=$disable?>> 
								Exploração de acervo da biblioteca;
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao8" name="oprid[96]"  value="t"   <?=(($oprid[96]=="t")?"checked":"")  ?> <?=$disable?>> 
								Saraus literários;
								</p>
							</td>
						</tr>
							<td>
								<p><input type="checkbox" class="questao8" name="oprid[94]"  value="t"  <?=(($oprid[94]=="t")?"checked":"")  ?> <?=$disable?>> 
								Jogos dramáticos/ teatrais;
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao8" name="oprid[97]"  value="t"  <?=(($oprid[97]=="t")?"checked":"")  ?> <?=$disable?>> 
								Produção de livros/ poesias/ contos/ cordel/ letras musicais e etc;
								</p>
							</td>
						<tr>
							<td>
								<p><input type="checkbox" class="questao8" name="oprid[95]"  value="t"  <?=(($oprid[95]=="t")?"checked":"")  ?> <?=$disable?>> 
								Contação de histórias;
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao8" name="oprid[98]"  value="t"  <?=(($oprid[98]=="t")?"checked":"")  ?> <?=$disable?>> 
								Realização de pesquisa e/ou mapeamento do patrimônio cultural material e/ou imaterial;
								</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "3">
					<p><b>
					9. Assinale o público principal envolvido nas atividades do Mais Cultura nas Escolas:
					</b></p>
				</td>
			</tr>
			<tr>
				<td colspan = "2">
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center"> 
						<tr>
							<td class="SubTituloCentro">
								Lista de Estudantes atendidos (referencia do CENSO 2014)
							</td>
						</tr>
						<tr>
							<td>
								<?
									$sql = "
											SELECT
												nie.descricao,
												'<input type=\"text\"  title=\"Quantidade\" id=\"qtdmatriculados['||lea.leaid||']\" class=\"disabled\"													
														value=\"'||qtdmatriculados||'\" maxlength=\"14\" size=\"16\" style=\"text-align:center;\">' as inp2,											 
												'<input type=\"text\"  title=\"Alunos\" id=\"alunosatendidos['||lea.leaid||']\" onblur=\"MouseBlur(this);\" $disable
													onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###\',this.value);\"
														class = \"testavalor\" value=\"'||COALESCE(to_char(alunosatendidos,'999'),'')||'\" maxlength=\"14\" size=\"16\" idlista=\"'||lea.leaid||'\" name=\"alunosatendidos['||lea.leaid||']\" style=\"text-align:center;\">' as inp
											FROM
												minc.listaestudantesatendidos lea 
											INNER JOIN minc.nivelescolaridade nie ON nie.nieid = lea.nieid
											WHERE
												mceid = {$_SESSION['minc']['mceid']}";
// 									ver($sql);
									$alinha = array("left","center","center");
									$cabecalho=array("Nível","Estudantes matriculados segundo o CENSO 2014","Alunos atendidos");
									$db->monta_lista($sql,$cabecalho,50,5,'N','center',$par2,'','',$alinha);
								?>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td colspan = "2">
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center"> 
						<tr>
							<td class="SubTituloCentro" colspan="2">
								Comunidade Escolar 
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita" width="46%">
								Familiares dos estudantes da escola
							</td>
							<td>
								<?php echo campo_texto('familiaresdosestudantes', 'N', $habilita, '', 10, 8, "####",'' , '', '', 0, 'id="familiaresdosestudantes" class="comunidade"','' , $familiaresdosestudantes); ?>
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita">
								Docentes da Escola
							</td>
							<td>
								<?php echo campo_texto('docentesdaescola', 'N', $habilita, '', 10, 8, "####",'' , '', '', 0, 'id="docentesdaescola" class="comunidade"','' , $docentesdaescola); ?>
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita" >
								Pessoas da Comunidade:
							</td>
							<td>
								<?php echo campo_texto('pessoasdacomunidade', 'N', $habilita, '', 10, 8, "####",'' , '', '', 0, 'id="pessoasdacomunidade" class="comunidade"','' , $pessoasdacomunidade); ?>
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita" >
								Outros(Outras pessoas que eventualmente se interessem e participem do projeto)
							</td>
							<td>
								<?php echo campo_texto('qtdoutros', 'N', $habilita, '', 10, 8, "####",'' , '', '', 0, 'id="qtdoutros" class="comunidade"','' , $qtdoutros); ?>
							</td>
						</tr>
						<tr>
							<td colspan ="2" align="right">
								Total de pessoas atendidas: <span id = "totalcomunidade"><?echo $totalcomunidade ?></span>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "2">
					<p><b>
					10. Quais materiais permanentes e equipamentos foram adquiridos com os recursos de CAPITAL?
					</b></p>
				</td>
			</tr>
			<tr>
				<td colspan = "2">
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td >
								<p><input type="checkbox" class="questao10" name="oprid[99]"  value="t"   <?=(($oprid[99]=="t")?"checked":"")  ?> <?=$disable?>> 
								Equipamentos circenses
								</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao10" name="oprid[102]"  value="t"   <?=(($oprid[102]=="t")?"checked":"")  ?> <?=$disable?>> 
								Equipamentos de informática (computador, impressora, tablet, scanner, software)
								</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao10" name="oprid[105]"  value="t"   <?=(($oprid[105]=="t")?"checked":"")  ?> <?=$disable?>> 
								Mobiliário em geral
								</p>
							</td>
						</tr>
							<td>
								<p><input type="checkbox" class="questao10" name="oprid[100]"  value="t"  <?=(($oprid[100]=="t")?"checked":"")  ?> <?=$disable?>> 
								Coleções, materiais bibliográficos, livros, Dvds e/ou CDs gravados</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao10" name="oprid[103]"  value="t"  <?=(($oprid[103]=="t")?"checked":"")  ?> <?=$disable?>> 
								Equipamentos, instrumentos musicais e partituras
								</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao10" name="oprid[106]"  value="t"  <?=(($oprid[106]=="t")?"checked":"")  ?> <?=$disable?>> 
								Outros materiais permanentes
								</p>
							</td>
						<tr>
							<td>
								<p><input type="checkbox" class="questao10" name="oprid[101]"  value="t"  <?=(($oprid[101]=="t")?"checked":"")  ?> <?=$disable?>> 
								Equipamento audiovisual (filmadora, câmeras fotográfica, projetor)</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao10" name="oprid[104]"  value="t"  <?=(($oprid[104]=="t")?"checked":"")  ?> <?=$disable?>> 
								Materiais para manutenção e/ou adequação de espaços</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "2">
					<p><b>
					11. Quais materiais e/ou ações foram possíveis com a utilização dos recursos de CUSTEIO?
					</b></p>
				</td>
			</tr>
			<tr>
				<td colspan = "2">
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td >
								<p><input type="checkbox" class="questao11" name="oprid[107]"  value="t"   <?=(($oprid[107]=="t")?"checked":"")  ?> <?=$disable?>> 
								Aquisição de CD-ROM virgem e/ou cartão de memória
								</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao11" name="oprid[113]"  value="t"   <?=(($oprid[113]=="t")?"checked":"")  ?> <?=$disable?>> 
								Contratação de transporte para atividades pedagógicas e culturais
								</p>
							</td>
						</tr>
							<td>
								<p><input type="checkbox" class="questao11" name="oprid[108]"  value="t"  <?=(($oprid[108]=="t")?"checked":"")  ?><?=$disable?>> 
								Aquisição de cartuchos para impressora e/ou contratação de serviços gráficos</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao11" name="oprid[114]"  value="t"  <?=(($oprid[114]=="t")?"checked":"")  ?> <?=$disable?>> 
								Contratação de serviços para manutenção e adequação de espaços
								</p>
							</td>
						<tr>
							<td>
								<p><input type="checkbox" class="questao11" name="oprid[109]"  value="t"  <?=(($oprid[109]=="t")?"checked":"")  ?> <?=$disable?>> 
								Aquisição de maquiagens, tecidos, tintas e/ou spray</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao11" name="oprid[115]"  value="t"  <?=(($oprid[115]=="t")?"checked":"")  ?> <?=$disable?>> 
								Locação de instrumentos e equipamentos de sonorização</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao11" name="oprid[110]"  value="t"  <?=(($oprid[110]=="t")?"checked":"")  ?> <?=$disable?>> 
								Aquisição de materiais didáticos, escolares e de expediente</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao11" name="oprid[116]"  value="t"  <?=(($oprid[116]=="t")?"checked":"")  ?> <?=$disable?>> 
								Pagamento de entrada em museus, teatros, cinemas, circos, etc.</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao11" name="oprid[111]"  value="t"  <?=(($oprid[111]=="t")?"checked":"")  ?> <?=$disable?>> 
								Contratação de serviços de áudio, vídeo e fotografia (produção, gravação, edição e/ou impressão)</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao11" name="oprid[117]"  value="t"  <?=(($oprid[117]=="t")?"checked":"")  ?> <?=$disable?>> 
								Pagamento da Iniciativa Cultural Parceira</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao11" name="oprid[112]"  value="t"  <?=(($oprid[112]=="t")?"checked":"")  ?> <?=$disable?>> 
								Contratação de serviços para a confecção de cenários e figurinos</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao11" name="oprid[118]"  value="t"  <?=(($oprid[118]=="t")?"checked":"")  ?> <?=$disable?>> 
								Pagamento de outros artistas, mestres populares, professores e/ou palestrantes, integrados no projeto cultural.</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "2">
					<p><b>
					12.Em que período escolar as atividades culturais aconteceram / acontecem?
					</b></p>
				</td>
			</tr>
			<tr>
				<td colspan = "2">
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td >
								<p><input type="checkbox" class="questao12" name="oprid[119]"  value="t"   <?=(($oprid[119]=="t")?"checked":"")  ?> <?=$disable?>> 
								No horário das aulas, sem integrar-se a alguma disciplina
								</p>
							</td>
							<td >
								<p><input type="checkbox" class="questao12" name="oprid[122]"  value="t"   <?=(($oprid[122]=="t")?"checked":"")  ?> <?=$disable?>> 
								Intervalos das aulas
								</p>
							</td>
						</tr>
							<td>
								<p><input type="checkbox" class="questao12" name="oprid[120]"  value="t"  <?=(($oprid[120]=="t")?"checked":"")  ?> <?=$disable?>> 
								Integrada a uma disciplina no horário normal de aula</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao12" name="oprid[123]"  value="t"  <?=(($oprid[123]=="t")?"checked":"")  ?> <?=$disable?>> 
								Aos finais de semana
								</p>
							</td>
						<tr>
							<td>
								<p><input type="checkbox" class="questao12" name="oprid[121]"  value="t"  <?=(($oprid[121]=="t")?"checked":"")  ?> <?=$disable?>> 
								No contraturno escolar</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "3">
					<p><b>
					13.Houve seleção do público participante?
					</b></p>
				</td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda">
					<p><input type="radio" class="selecaopublico" name="selecaopublico_13"  value="1"  <?=(($queid[17] == 't')?"checked":"")  ?> <?=$disable?>>Sim
					<input type="radio" class="selecaopublico" name="selecaopublico_13"  value="2"   <?=(($queid[16] == 'f')?"checked":"")  ?> <?=$disable?>>Não</p>
				</td>
			</tr>
			<tr id="tr_selecaopublico" <?= (($queid[17]  == 't') ? "" : "style=\"display:none\"") ?>>
				<td>
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td width="8%">
								De acordo com:
							</td>
							<td align="Left">
								<? 
									$sql = "SELECT
												oprid as codigo,
												oprdescricao as descricao
											FROM
												minc.opcaoresposta
											WHERE
												queid = 17 ORDER BY descricao";
									$db->monta_combo("publico", $sql, $habilita, Selecione,'' ,'' ,'' ,'' , 'S', $publico , null, null, null, '');
								?>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "3">
					<p><b>
					14. O projeto viabilizado pelo Mais Cultura está articulado a outros projetos ou atividades desenvolvidas pela escola?
					</b></p>
				</td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda">
					<p><input type="radio" class="articuladooutrosprojetos" name="articuladooutrosprojetos_14"  value="1" <?=(($queid[19] == 't')?"checked":"")  ?> <?=$disable?>>Sim
					<input type="radio" class="articuladooutrosprojetos" name="articuladooutrosprojetos_14"  value="2"  <?=(($queid[18] == 'f')?"checked":"")  ?> <?=$disable?>>Não</p>
				</td>
			</tr>
			<tr id="tr_articuladooutrosprojetos" <?= (($queid[19] == 't') ? "" : "style=\"display:none\"") ?>>
				<td>
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td width="14%">
								Programas e projetos da esfera:
							</td>
							<td align="Left">
									<p><input type="checkbox" class="questao14" name="oprid[133]"  value="t"  <?=(($oprid[133]=="t")?"checked":"")  ?> <?=$disable?>> 
								Municipal &nbsp; 
									<input type="checkbox" class="questao14" name="oprid[134]"  value="t"  <?=(($oprid[134]=="t")?"checked":"")  ?> <?=$disable?>> 
								Estadual &nbsp; 
									<input type="checkbox" class="questao14" name="oprid[135]"  value="t"  <?=(($oprid[135]=="t")?"checked":"")  ?> <?=$disable?>> 
								Federal</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "2">
					<p><b>
					15. Como se deu a relação de parceria entre escola e Iniciativa Cultural.
					</b></p>
				</td>
			</tr>
			<tr>
				<td colspan = "2">
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td width="50%">
								<p><input type="checkbox" class="questao15" name="oprid[136]"  value="t"   <?=(($oprid[136]=="t")?"checked":"")  ?> <?=$disable?>> 
								De forma harmoniosa e respeitosa;
								</p>
							</td>
							<td width="50%">
								<p><input type="checkbox" class="questao15" name="oprid[138]"  value="t"   <?=(($oprid[138]=="t")?"checked":"")  ?> <?=$disable?>> 
								Conflituosa em relação ao pagamento dos serviços culturais;
								</p>
							</td>
						</tr>
						<tr>
							<td>
								<p><input type="checkbox" class="questao15" name="oprid[137]"  value="t"  <?=(($oprid[137]=="t")?"checked":"")  ?> <?=$disable?>> 
								De forma harmoniosa, respeitosa, integrada e articulada em todo o processo, desde a construção do Plano Cultural até o desenvolvimento das atividades;</p>
							</td>
							<td>
								<p><input type="checkbox" class="questao15" name="oprid[139]"  value="t"  <?=(($oprid[139]=="t")?"checked":"")  ?> <?=$disable?>> 
								Conflituosa em relação ao calendário;
								</p>
							</td>
						</tr>	
						<tr>
							<td>
							</td>
							<td>
								<p><input type="checkbox" class="questao15" name="oprid[140]"  value="t"  <?=(($oprid[140]=="t")?"checked":"")  ?> <?=$disable?>> 
								Conflituosa em relação aos aspectos pedagógicos.
								</p>
							</td>
						</tr>	
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "3">
					<p><b>
					16.O desenvolvimento do Mais Cultura nas Escolas possibilitou a criação e/ou utilização de espaços virtuais para compartilhamento 
					de informações sobre as atividades culturais (blogs, redes sociais, youtube, páginas virtuais, etc.)?
					</b></p>
				</td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda">
					<p><input type="radio" class="utilizouespacovirtual" name="utilizouespacovirtual_16"  value="1"  <?=($queid[21]!=''?(($queid[21] == 't')?"checked":""):"")  ?> <?=$disable?>>Sim
					<input type="radio" class="utilizouespacovirtual" name="utilizouespacovirtual_16"  value="2"  <?=($queid[21]!=''?(($queid[21] != 't')?"checked":""):"")  ?> <?=$disable?>>Não</p>
				</td>
			</tr>
			<tr id="tr_utilizouespacovirtual" <?= (($queid[21] == 't') ? "" : "style=\"display:none\"") ?>>
				<td>
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td>
								Link<?php echo campo_texto('link1', 'N', $habilita, '', 70,100, '','' , '', '', 0, 'name="link1"' , '', $total_meta); ?>
							</td>
							<td>
								Link<?php echo campo_texto('link2','N', $habilita, '', 70,100, '','' ,'' ,'' , 0, 'name="link2"' ,'' , $total_meta); ?>
							</td>
							<td>
								Link<?php echo campo_texto('link3', 'N', $habilita, '', 70,100, '','' ,'' ,'' , 0, 'name="link3"' ,'', $total_meta); ?>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#e7e7e7">
				<td class="SubTituloEsquerda" style="font-size: 15px" colspan = "3">
					<p><b>
					17.Este espaço é destinado à inclusão de fotos produzidas no desenvolvimento do PMCE.
					</b></p>
				</td>
			</tr>
			<tr>
				<td>
					<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
	                        <td align="left" width="10%">1° Nome do Autor/Arquivo:</td>
	                        <td width="90%">
	                            <?php if (isset($arquivo[0]['arqid']) && !empty($arquivo[0]['arqid'])): ?>
	                            	<input type="hidden" name="arquivo" id="arquivo" value="uploaded" />
	                            <?/*if($habilita==S){*/?>
	                            	<img src="../imagens/excluir.gif" style=cursor:pointer; <?=$disable?> onclick="excluirAnexo(<?php echo $arquivo[0]['arqid']; ?>);" />&nbsp;
	                            <?/*}*/?>
	                            <a href="minc.php?modulo=principal/monitoramento2&acao=A&requisicao=downloadArquivo&arqid=<?php echo $arquivo[0]['arqid']; ?>">
	                                <?php echo "Autor:{$arquivo[0]['autor']}.{$arquivo[0]['arqnome']}.{$arquivo[0]['arqextensao']}"; ?>
	                            </a>
	                            <?php else: ?>
	                          		<?echo campo_texto("arquivo[autor][1]", 'N', $habilita, 'Digite no nome do autor aqui.', 50,100, '','' , '', '', 0, 'name="arquivo1autor"' , '','', '', Array(';float:left')); ?>
	                          		<input type="file" name="arquivo1" <?=$disable?> />
	                            <?php endif; ?>
	                        </td>
                    	</tr>
						<tr>
	                        <td align="left">2° Nome do Autor/Arquivo:</td>
	                        <td align="left">
	                            <?php if (isset($arquivo[1]['arqid']) && !empty($arquivo[1]['arqid'])): ?>
	                            <?/*if($habilita==S){*/?>
	                            	<img src="../imagens/excluir.gif" style=cursor:pointer; <?=$disable?> onclick="excluirAnexo(<?php echo $arquivo[1]['arqid']; ?>);" />&nbsp;
	                            <?/*}*/?>
	                            <a href="minc.php?modulo=principal/monitoramento2&acao=A&requisicao=downloadArquivo&arqid=<?php echo $arquivo[1]['arqid']; ?>">
	                                <?php echo "Autor:{$arquivo[1]['autor']}.{$arquivo[1]['arqnome']}.{$arquivo[1]['arqextensao']}"; ?>
	                            </a>
	                            <?php else: ?>
	                            <?echo campo_texto("arquivo[autor][2]", 'N', $habilita, 'Digite no nome do autor aqui.', 50,100, '','' , '', '', 0, 'name="arquivo2autor"' , '', '', '', Array(';float:left')); ?>
	                            <input type="file" name="arquivo2"  <?=$disable?> />
	                            <?php endif; ?>
	                        </td>
                    	</tr>
						<tr>
	                        <td align="left">3° Nome do Autor/Arquivo:</td>
	                        <td align="left" >
	                            <?php if (isset($arquivo[2]['arqid']) && !empty($arquivo[2]['arqid'])): ?>
	                            <?/*if($habilita==S){*/?>
	                            	<img src="../imagens/excluir.gif" style=cursor:pointer; <?=$disable?> onclick="excluirAnexo(<?php echo $arquivo[2]['arqid']; ?>);" />&nbsp;
	                            <?/*}*/?>
	                            <a href="minc.php?modulo=principal/monitoramento2&acao=A&requisicao=downloadArquivo&arqid=<?php echo $arquivo[2]['arqid']; ?>">
	                                <?php echo "Autor:{$arquivo[2]['autor']}.{$arquivo[2]['arqnome']}.{$arquivo[2]['arqextensao']}"; ?>
	                            </a>
	                            <?php else: ?>
	                            <?echo campo_texto("arquivo[autor][3]", 'N', $habilita, 'Digite no nome do autor aqui.', 50,100, '','' , '', '', 0, 'name="arquivo3autor"' , '', '', '', Array(';float:left')); ?>
	                            <input type="file" name="arquivo3" <?=$disable?>/>
	                            <?php endif; ?>
	                        </td>
                    	</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td align="center" colspan="2">
					<input type="button" name="salvar" value="Salvar" class="salvar""> &nbsp;&nbsp;
					<input type="button" name="fechar" value="Fechar" class="fechar"">
				</td>
			</tr>
		</table>
	</form>
