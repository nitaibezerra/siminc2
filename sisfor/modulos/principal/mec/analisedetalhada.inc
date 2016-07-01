<?php 

$mensarios = recuperarMensarios($_GET['sifid'], $_GET['fpbid']);
 

$sqlOrientador = "    select iusnome as descricao
                                                                        from sisfor.identificacaousuario iu
                                                                            inner join sisfor.tipoperfil tp on tp.iusd = iu.iusd
                                                                        where sifid = '{$_GET['sifid']}'
                                                                        order by descricao"; 


$sql = "select u.uniabrev ||' - '|| unidsc as uninome,
            case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc
			     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc
			     when s.ocuid is not null then oc.ocunome end as curdesc,
			    iusnome,
				case when s.ieoid is not null then cor.coordsigla
				     when s.cnvid is not null then cor2.coordsigla
				     when s.ocuid is not null then cor3.coordsigla end as secretaria,
    			to_char(sifvigenciadtini,'dd/mm/YYYY') as sifvigenciadtini,
    			to_char(sifvigenciadtfim,'dd/mm/YYYY') as sifvigenciadtfim
				     
        from sisfor.sisfor s
                inner join public.unidade u on u.unicod = s.unicod and u.unitpocod = s.unitpocod
                left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
                left join catalogocurso2014.curso cur on cur.curid = ieo.curid AND cur.curstatus = 'A'
                left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
                left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
                left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid AND cur2.curstatus = 'A'
                left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
                left join seguranca.usuario usu on usu.usucpf = s.usucpf
                left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
                left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid
                inner join sisfor.folhapagamentoprojeto fpp on s.sifid = fpp.sifid
                INNER JOIN sisfor.folhapagamento f ON f.fpbid = fpp.fpbid
                inner join workflow.documento doc on doc.docid = fpp.docid
				inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
				left join workflow.historicodocumento hst on hst.hstid = doc.hstid
				left join seguranca.usuario uu on uu.usucpf = hst.usucpf
                INNER JOIN sisfor.identificacaousuario i ON i.iuscpf=s.usucpf
            where s.sifid='{$_GET['sifid']}'";

$arrCurso = $db->pegaLinha($sql);


$perbolsistas = $db->pegaLinha("select to_char(min((fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,4,'0')||'-01')::date),'mm/YYYY') as inicio, to_char(max((fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,4,'0')||'-01')::date),'mm/YYYY') as fim from sisfor.folhapagamentoprojeto fp
								     inner join sisfor.folhapagamento f on f.fpbid = fp.fpbid
								     where fp.sifid='".$_GET['sifid']."'");

$percursistas = $db->pegaLinha("select to_char(min((fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,4,'0')||'-01')::date),'mm/YYYY') as inicio, to_char(max((fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,4,'0')||'-01')::date),'mm/YYYY') as fim from sisfor.folhapagamentocursista fc
								     inner join sisfor.folhapagamento f on f.fpbid = fc.fpbid
								     where fc.sifid='".$_GET['sifid']."'");


?>

<table width="100%">
<tr>
	<td valign="top">

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">
        <tr>
            <td class="SubTituloDireita">Nome da IES</td>
            <td colspan="5"><?php echo $arrCurso['uninome']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome do curso</td>
            <td colspan="5"><?php echo $arrCurso['curdesc']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome do Coordenador Curso</td>
            <td colspan="5"><?php echo $arrCurso['iusnome']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Secretaria</td>
            <td colspan="5"><?php echo $arrCurso['secretaria']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Mês de referência</td>
            <td colspan="5"><?php
             
            echo $db->pegaUm("SELECT m.mesdsc || ' / ' || fpbanoreferencia as descricao
						FROM sisfor.folhapagamento f
						INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
						WHERE f.fpbid='{$_GET['fpbid']}'");

            ?></td>
        </tr>
    <tr>
    	<td class="SubTituloDireita" width=20%>Vigência do projeto</td><td><?=(($arrCurso['sifvigenciadtini'] && $arrCurso['sifvigenciadtfim'])?$arrCurso['sifvigenciadtini'] . ' até ' . $arrCurso['sifvigenciadtfim']:'-') ?></td>
    	<td class="SubTituloDireita" width=20%>Período de referência dos bolsistas</td><td><?=(($perbolsistas['inicio'] && $perbolsistas['fim'])?$perbolsistas['inicio'] . ' até ' . $perbolsistas['fim']:'Não cadastrado') ?></td>
    	<td class="SubTituloDireita" width=20%>Período de referência dos cursistas</td><td><?=(($percursistas['inicio'] && $percursistas['fim'])?$percursistas['inicio'] . ' até ' . $percursistas['fim']:'Não cadastrado') ?></td>
    </tr>
        

    </table>
    
    </td>
    <td width="3%" valign="top">
	<?php
	$docid = pegarDocidFolhaPagamentoProjeto($_GET['sifid'], $_GET['fpbid']);
	$estadoAtual = wf_pegarEstadoAtual($docid);
	$estadoAtual[esddsc];
	wf_desenhaBarraNavegacao($docid, array('sifid' => $_GET['sifid'], "fpbid" => $_GET['fpbid']));
	?>
    </td>
</tr>
</table>

<div style="font-weight: bold;text-align: center;text-align: center;font-size: 14px;padding: 20px">Síntese geral do curso</div> 

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
<td>
<?	
	$sql = "select p.pfldsc, 
				   epiqtd, 
				   (select count(*) from sisfor.pagamentobolsista pg 
					inner join sisfor.tipoperfil tt on tt.tpeid = pg.tpeid 
					inner join workflow.documento d on d.docid = pg.docid  
					where pg.pflcod=p.pflcod and tt.sifid=e.sifid and d.esdid!=".ESD_PAGAMENTO_NAO_AUTORIZADO.") as qtdutilizada 
			from sisfor.equipeies e
	 	    inner join seguranca.perfil p on p.pflcod = e.pflcod 
	 	    where sifid='".$_GET['sifid']."'";
	
	$cabecalho = array('Perfil','Qtd disponível','Qtd utilizada');
	$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%');
?>
</td>
</tr>
	
<tr>
<td>
<? 

include_once APPRAIZ . "includes/library/simec/Grafico.php"; 

$grafico = new Grafico();

$sql = "select per.pfldsc as descricao, m.mesdsc || ' / ' || fpbanoreferencia as categoria, count(*) as valor
		from sisfor.pagamentobolsista p 
		inner join sisfor.tipoperfil t on t.tpeid = p.tpeid 
		inner join seguranca.perfil per on per.pflcod = t.pflcod 
		inner join sisfor.folhapagamento f on f.fpbid = p.fpbid
		inner join public.meses m ON m.mescod::integer = f.fpbmesreferencia
		where sifid=".$_GET['sifid']." 
		group by per.pfldsc, m.mesdsc, f.fpbanoreferencia, per.pflnivel, f.fpbid
		order by f.fpbid, per.pflnivel";

$grafico->setTitulo('Pagamentos de bolsas por perfil/mês')->setTipo(Grafico::K_TIPO_COLUNA)->setWidth('100%')->gerarGrafico($sql);

?>



</td>
</tr>
	
</table>


<div style="font-weight: bold;text-align: center;text-align: center;font-size: 14px;padding: 20px">Situação dos bolsistas</div>

<table width="100%">
    <tr><td width="90%">
            <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem tabela" style="margin-top: 10px;">
                <thead>
                    <tr align="center">
                        <td>CPF</td>
                        <td>Nome</td>
                        <td>E-mail</td>
                        <td>Perfil</td>
                        <td>Atividades Realizadas</td>
                        <td>Avaliado por</td>
                        <td>Situação</td>
                    </tr>
                </thead>
                <tbody>
                    <?php
                    if ($mensarios && is_array($mensarios)) {
                        foreach ($mensarios as $count => $dado) {
                            ?>
                            <tr>
                                <td>

                                    <?php echo $dado['iuscpf'] ?>
                                </td>
                                <td><?php echo $dado['iusnome'] ?></td>
                                <td><?php echo $dado['iusemailprincipal'] ?></td>
                                <td><?php echo $dado['pfldsc'] ?></td>
                                <td align="center">
                                    <?php
                                    switch ($dado['mavatividadesrealizadas']) {
                                        case 'A':
                                        	$total[$dado['pfldsc']]['A']++;
                                            echo "Realizou as atividades";
                                            break;
                                        case 'I':
                                        	$total[$dado['pfldsc']]['I']++;
                                            echo "Não realizou as atividades";
                                            break;
                                        case 'N':
                                        	$total[$dado['pfldsc']]['N']++;
                                            echo "Não se aplica";
                                            break;
                                    }
                                    ?>
                                </td>
                                <td align="center">
                                    <?php echo $dado['iusnomeavaliador']; ?>  
                                </td>
                                <td align="center">
                                    <?php if ('I' == trim($dado['mavatividadesrealizadas']) || 'N' == trim($dado['mavatividadesrealizadas'])) { ?>
                                        <span style="color:red;">Inapto</span>
                                        <?php echo (($dado['motivoavaliacao'])?"<span style=font-size:x-small;><b>Motivo</b> : ".$dado['motivoavaliacao']."</font>":""); ?>
                                    <?php } else { ?>
                                        <span style="color:blue;">Apto</span>
                                    <?php } ?>
                                </td>
                            </tr>
                        <?php } ?>
                        <tr>
                            <td align="right"colspan="9">
                            
							<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
								<tr>
									<td class="SubTituloCentro">Função</td>
									<td class="SubTituloCentro">Realizou atividades</td>
									<td class="SubTituloCentro">Não realizou atividades</td>
									<td class="SubTituloCentro">Não se aplica</td>
								</tr>
							<? if($total) : ?>
							<? foreach($total as $pfldsc => $t) : ?>
								<tr>
									<td class="SubTituloDireita"><?=$pfldsc ?></td>
									<td align="right"><?=(($t['A'])?$t['A']:'0') ?></td>
									<td align="right"><?=(($t['I'])?$t['I']:'0') ?></td>
									<td align="right"><?=(($t['N'])?$t['N']:'0') ?></td>
								</tr>
								<? $totalfim['A'] += $t['A']; ?>
								<? $totalfim['I'] += $t['I']; ?>
								<? $totalfim['N'] += $t['N']; ?>
							<? endforeach; ?>
							<? endif; ?>
							
							<tr>
								<td class="SubTituloDireita"><b>TOTAL:</b></td>
								<td align="right"><?=(($totalfim['A'])?$totalfim['A']:'0') ?></td>
								<td align="right"><?=(($totalfim['I'])?$totalfim['I']:'0') ?></td>
								<td align="right"><?=(($totalfim['N'])?$totalfim['N']:'0') ?></td>
							</tr>
							
							
							</table>
							
                            </td>
                        </tr>
                    <?php } else { ?>
                        <tr bgcolor="" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#ffffcc';">
                            <td colspan="4" align="center"><span style="color: red;">Não foi encontrado planejamento de equipe</span></td>
                        </tr>
                    <?php } ?>
                </tbody>
            </table>
            
            <div style="font-weight: bold;text-align: center;text-align: center;font-size: 14px;padding: 20px">Situação dos pagamentos por bolsista</div>
            
			<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
			<td class="SubTituloCentro">Bolsista</td>
			<td class="SubTituloCentro">Perfil</td>
			<td class="SubTituloCentro">Pagamento gerado</td>
			<td class="SubTituloCentro">Situação pagamento</td>
			<td class="SubTituloCentro">Restrição</td>
			</tr>
			<?
			if($mensarios[0]) {
				foreach($mensarios as $me) {
		
					if($me['mavatividadesrealizadas']=='A') {
						$vagas = $db->pegaLinha("SELECT tpebolsa, tpeqtdbolsa as qtdprevista, (select count(*) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid = p.docid where p.tpeid=t.tpeid and d.esdid!=".ESD_PAGAMENTO_NAO_AUTORIZADO.") as qtdutilizada FROM sisfor.tipoperfil t WHERE tpeid='".$me['tpeid']."'");
						
						$pagamento = $db->pegaLinha("SELECT p.pboid, e.esddsc FROM sisfor.pagamentobolsista p 
													 INNER JOIN workflow.documento d ON d.docid = p.docid 
													 INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
												     INNER JOIN sisfor.tipoperfil t ON t.tpeid = p.tpeid  
												     WHERE p.iusd='".$me['iusd']."' AND p.fpbid='".$me['fpbid']."' AND t.tpeid='".$me['tpeid']."'");
						unset($restricao);
						$termocompromisso = $db->pegaUm("SELECT iustermocompromisso FROM sisfor.identificacaousuario WHERE iusd='".$me['iusd']."'");
						
		
						
						if ($termocompromisso!='t') {
							$restricao .= '<span style="font-size:x-small;color:red;">Termo de compromisso não aceito</span>';
						} elseif($vagas['tpebolsa']!='t') {
							$restricao .= '<span style="font-size:x-small;color:red;">Não deseja receber bolsa do curso</span>';
						} elseif(!$pagamento) {
							$erros = verificarCriacaoPagamento(array('tpeid' => $me['tpeid'],'fpbid' => $_GET['fpbid']));
							
							if($erros) {
								$restricao .= '<span style="font-size:x-small;color:red;">';
								$restricao .= implode("<br>",$erros);
			        			$restricao .= '</span>';
		        			}
						}
		
						echo '<tr>';
						echo '<td>'.$me['iuscpf'].' - '.$me['iusnome'].'</td>';
						echo '<td>'.$me['pfldsc'].'</td>';
						echo '<td align="center">'.(($pagamento)?'<span style="font-size:x-small;color:blue;">Sim</span>':'<span style="font-size:x-small;color:red;">Não</span>').' <span style="font-size:x-small;"><b>( '.(($vagas['qtdutilizada'])?$vagas['qtdutilizada']:'0').' / '.(($vagas['qtdprevista'])?$vagas['qtdprevista']:'0').' )</b></span></td>';
						echo '<td align="center">'.(($pagamento)?'<span style="color:blue;font-size:x-small;">'.$pagamento['esddsc'].'</span>':'<span style="color:red;font-size:x-small;">Pagamento não foi gerado</span>').'</td>';
						echo '<td align="center" width="30%">'.(($restricao)?$restricao:'<span style="font-size:x-small;">Não possui restrições</span>').'</td>';
						echo '<tr>';
					
					}
				}
			}
			?>
			</table>


            <?php
            $sql = "select c.curid, c.curnome, c.curcpf, c.curemail, cc.cucid, a.cavid, a.cavparticipacao,a.cavsituacao, a.cavatividadesrealizadas, a.iusdorientador, a.cavtotal, cc.iusdavaliador, cc.estuf, cc.muncod
            from sisfor.cursistacurso cc
                inner join sisfor.cursista c on c.curid = cc.curid
                left  join sisfor.cursistaavaliacoes a on a.cucid = cc.cucid
            where cc.sifid = '{$_GET['sifid']}'
            and cc.fpbid = '{$_GET['fpbid']}' ORDER BY cc.iusdavaliador, c.curnome";

            $dadoscurs = $db->carregar($sql);

            $sqlOrientador = "  select iu.iusd as codigo, iusnome as descricao
                        from sisfor.identificacaousuario iu
                            inner join sisfor.tipoperfil tp on tp.iusd = iu.iusd
                        where sifid = '{$_GET['sifid']}'
                        order by descricao";

            $dadosOrientador = $db->carregar($sqlOrientador);
            ?>


            <div style="font-weight: bold;text-align: center;text-align: center;font-size: 14px;padding: 20px">Situação dos cursistas</div>

            <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem tabela" style="margin-top: 10px;">
                <thead>
                    <tr align="center">
                        <td>CPF</td>
                        <td>Nome</td>
                        <td>E-mail</td>
                        <td>Participação</td>
                        <td>Atividades Realizadas</td>
                        <td>Situação</td>
                        <td>Avaliador</td>
                    </tr>
                </thead>
                <tbody>
                    <?php
                    if ($dadoscurs && is_array($dadoscurs)) {
                        foreach ($dadoscurs as $count => $dado) {
                            ?>
                            <tr 
                            <?

                            switch($dado['cavsituacao']) {
								case 'r':
								case 'e':
								case 'f':
									echo 'style="background-color: #FF7171"';
									break;
								case 'p':
									echo 'style="background-color: #B2B2FF"';
									break;
							}
							
							?>
                            >
                                <td>
                                    <input type="hidden" name="avaliacao[<?php echo $count; ?>][curid]" value="<?php echo $dado['curid'] ?>" />
                                    <input type="hidden" name="avaliacao[<?php echo $count; ?>][cucid]" value="<?php echo $dado['cucid'] ?>" />
                                    <input type="hidden" name="avaliacao[<?php echo $count; ?>][cavid]" value="<?php echo $dado['cavid'] ?>" />
                                    <?php echo $dado['curcpf'] ?>
                                </td>
                                <td><?php echo $dado['curnome'] ?></td>
                                <td><?php echo $dado['curemail'] ?></td>
                                <td align="center">
                                    <?php
                                    $cavparticipacao = ('0' === $dado['cavparticipacao']) ? '0.0' : number_format($dado['cavparticipacao'], '1', ',', '.');


                                    switch ($cavparticipacao) {
                                        case '5,0':
                                            echo "Participou integralmente";
                                            break;
                                        case '3,5':
                                            echo "Participou suficientemente";
                                            break;
                                        case '2,0':
                                            echo "Participou insuficientemente";
                                            break;
                                        case '0':
                                            echo "Não Participou";
                                            break;
                                    }
                                    ?>
                                </td>
                                <td align="center">
                                    <?php
                                    switch ($dado['cavatividadesrealizadas']) {
                                        case '5.0':
                                            echo "Realizou as atividades integralmente";
                                            break;
                                        case '3.5':
                                            echo "Realizou as atividades suficientemente";
                                            break;
                                        case '2.0':
                                            echo "Realizou as atividades insuficientemente";
                                            break;
                                        case '0':
                                            echo "Não realizou as atividades";
                                            break;
                                    }
                                    ?>
                                </td>

                                <td align="center"> 

        <?php
        
        switch ($dado['cavsituacao']) {
            case 'e':
                echo "Evadido";
                $totalc['e']++;
                $totalcc['e']['participacao'][$cavparticipacao]++;
                $totalcc['e']['atividade'][$dado['cavatividadesrealizadas']]++;
                break;
            case 'f':
                echo "Faleceu";
                $totalc['f']++;
                $totalcc['f']['participacao'][$cavparticipacao]++;
                $totalcc['f']['atividade'][$dado['cavatividadesrealizadas']]++;
                break;
            case 'c':
                echo "Cursando";
                $totalc['c']++;
                $totalcc['c']['participacao'][$cavparticipacao]++;
                $totalcc['c']['atividade'][$dado['cavatividadesrealizadas']]++;
                break;
            case 'a':
              	echo "Matriculado";
              	$totalc['a']++;
                $totalcc['a']['participacao'][$cavparticipacao]++;
                $totalcc['a']['atividade'][$dado['cavatividadesrealizadas']]++;
               	break;
           	case 'b':
           		echo "Desvinculado";
           		$totalc['b']++;
           		$totalcc['b']['participacao'][$cavparticipacao]++;
           		$totalcc['b']['atividade'][$dado['cavatividadesrealizadas']]++;
           		break;
       		case 'd':
       			echo "Trancado";
       			$totalc['d']++;
       			$totalcc['d']['participacao'][$cavparticipacao]++;
       			$totalcc['d']['atividade'][$dado['cavatividadesrealizadas']]++;
       			break;
   			case 'p':
   				echo "Aprovado";
   				$totalc['p']++;
   				$totalcc['p']['participacao'][$cavparticipacao]++;
   				$totalcc['p']['atividade'][$dado['cavatividadesrealizadas']]++;
   				break;
			case 'r':
   				echo "Reprovado";
   				$totalc['r']++;
   				$totalcc['r']['participacao'][$cavparticipacao]++;
   				$totalcc['r']['atividade'][$dado['cavatividadesrealizadas']]++;
   				break;
                                
        }
        ?>




                                <td>
        <?php
        $aOrientador = array();
        foreach ($dadosOrientador as $orientador) {
            $aOrientador[$orientador['codigo']] = $orientador['descricao'];
        }
        echo $aOrientador[$dado['iusdavaliador']];
        ?>
                                </td>
                            </tr>
                                <? } ?>
                        <tr>
                            <td align="center" colspan="9">
                            
							<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
								<tr>
									<td class="SubTituloDireita">&nbsp;</td>
									<td class="SubTituloCentro">Participação</td>
									<td class="SubTituloCentro">Atividades realizadas</td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Evadido(s): <?=(($totalc['e'])?$totalc['e']:'0') ?></td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Participou integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['e']['participacao']['5,0'])?$totalcc['e']['participacao']['5,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['e']['participacao']['3,5'])?$totalcc['e']['participacao']['3,5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['e']['participacao']['2,0'])?$totalcc['e']['participacao']['2,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não Participou</td>
											<td style="font-size:x-small;"><?=(($totalcc['e']['participacao']['0.0'])?$totalcc['e']['participacao']['0.0']:'0') ?></td>
										</tr>
									</table>
									</td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['e']['atividade']['5.0'])?$totalcc['e']['atividade']['5.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['e']['atividade']['3.5'])?$totalcc['e']['atividade']['3.5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['e']['atividade']['2.0'])?$totalcc['e']['atividade']['2.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não realizou as atividades</td>
											<td style="font-size:x-small;"><?=(($totalcc['e']['atividade']['0'])?$totalcc['e']['atividade']['0']:'0') ?></td>
										</tr>
									</table>
									</td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Falecido(s): <?=(($totalc['f'])?$totalc['f']:'0') ?></td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Participou integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['f']['participacao']['5,0'])?$totalcc['f']['participacao']['5,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['f']['participacao']['3,5'])?$totalcc['f']['participacao']['3,5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['f']['participacao']['2,0'])?$totalcc['f']['participacao']['2,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não Participou</td>
											<td style="font-size:x-small;"><?=(($totalcc['f']['participacao']['0.0'])?$totalcc['f']['participacao']['0.0']:'0') ?></td>
										</tr>
									</table>
									</td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['f']['atividade']['5.0'])?$totalcc['f']['atividade']['5.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['f']['atividade']['3.5'])?$totalcc['f']['atividade']['3.5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['f']['atividade']['2.0'])?$totalcc['f']['atividade']['2.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não realizou as atividades</td>
											<td style="font-size:x-small;"><?=(($totalcc['f']['atividade']['0'])?$totalcc['f']['atividade']['0']:'0') ?></td>
										</tr>
									</table>
									</td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Cursando(s): <?=(($totalc['c'])?$totalc['c']:'0') ?></td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Participou integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['c']['participacao']['5,0'])?$totalcc['c']['participacao']['5,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['c']['participacao']['3,5'])?$totalcc['c']['participacao']['3,5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['c']['participacao']['2,0'])?$totalcc['c']['participacao']['2,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não Participou</td>
											<td style="font-size:x-small;"><?=(($totalcc['c']['participacao']['0.0'])?$totalcc['c']['participacao']['0.0']:'0') ?></td>
										</tr>
									</table>
									</td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['c']['atividade']['5.0'])?$totalcc['c']['atividade']['5.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['c']['atividade']['3.5'])?$totalcc['c']['atividade']['3.5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['c']['atividade']['2.0'])?$totalcc['c']['atividade']['2.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não realizou as atividades</td>
											<td style="font-size:x-small;"><?=(($totalcc['c']['atividade']['0'])?$totalcc['c']['atividade']['0']:'0') ?></td>
										</tr>
									</table>
									</td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Matriculado(s): <?=(($totalc['a'])?$totalc['a']:'0') ?></td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Participou integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['a']['participacao']['5,0'])?$totalcc['a']['participacao']['5,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['a']['participacao']['3,5'])?$totalcc['a']['participacao']['3,5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['a']['participacao']['2,0'])?$totalcc['a']['participacao']['2,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não Participou</td>
											<td style="font-size:x-small;"><?=(($totalcc['a']['participacao']['0.0'])?$totalcc['a']['participacao']['0.0']:'0') ?></td>
										</tr>
									</table>
									</td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['a']['atividade']['5.0'])?$totalcc['a']['atividade']['5.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['a']['atividade']['3.5'])?$totalcc['a']['atividade']['3.5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['a']['atividade']['2.0'])?$totalcc['a']['atividade']['2.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não realizou as atividades</td>
											<td style="font-size:x-small;"><?=(($totalcc['a']['atividade']['0'])?$totalcc['a']['atividade']['0']:'0') ?></td>
										</tr>
									</table>
									</td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Desvinculado(s): <?=(($totalc['b'])?$totalc['b']:'0') ?></td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Participou integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['b']['participacao']['5,0'])?$totalcc['b']['participacao']['5,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['b']['participacao']['3,5'])?$totalcc['b']['participacao']['3,5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['b']['participacao']['2,0'])?$totalcc['b']['participacao']['2,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não Participou</td>
											<td style="font-size:x-small;"><?=(($totalcc['b']['participacao']['0.0'])?$totalcc['b']['participacao']['0.0']:'0') ?></td>
										</tr>
									</table>
									</td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['b']['atividade']['5.0'])?$totalcc['b']['atividade']['5.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['b']['atividade']['3.5'])?$totalcc['b']['atividade']['3.5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['b']['atividade']['2.0'])?$totalcc['b']['atividade']['2.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não realizou as atividades</td>
											<td style="font-size:x-small;"><?=(($totalcc['b']['atividade']['0'])?$totalcc['b']['atividade']['0']:'0') ?></td>
										</tr>
									</table>
									</td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Trancado(s): <?=(($totalc['d'])?$totalc['d']:'0') ?></td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Participou integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['d']['participacao']['5,0'])?$totalcc['d']['participacao']['5,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['d']['participacao']['3,5'])?$totalcc['d']['participacao']['3,5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['d']['participacao']['2,0'])?$totalcc['d']['participacao']['2,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não Participou</td>
											<td style="font-size:x-small;"><?=(($totalcc['d']['participacao']['0.0'])?$totalcc['d']['participacao']['0.0']:'0') ?></td>
										</tr>
									</table>
									</td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['d']['atividade']['5.0'])?$totalcc['d']['atividade']['5.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['d']['atividade']['3.5'])?$totalcc['d']['atividade']['3.5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['d']['atividade']['2.0'])?$totalcc['d']['atividade']['2.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não realizou as atividades</td>
											<td style="font-size:x-small;"><?=(($totalcc['d']['atividade']['0'])?$totalcc['d']['atividade']['0']:'0') ?></td>
										</tr>
									</table>
									</td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Aprovado(s): <?=(($totalc['p'])?$totalc['p']:'0') ?></td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Participou integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['p']['participacao']['5,0'])?$totalcc['p']['participacao']['5,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['p']['participacao']['3,5'])?$totalcc['p']['participacao']['3,5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['p']['participacao']['2,0'])?$totalcc['p']['participacao']['2,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não Participou</td>
											<td style="font-size:x-small;"><?=(($totalcc['p']['participacao']['0.0'])?$totalcc['p']['participacao']['0.0']:'0') ?></td>
										</tr>
									</table>
									</td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['p']['atividade']['5.0'])?$totalcc['p']['atividade']['5.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['p']['atividade']['3.5'])?$totalcc['p']['atividade']['3.5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['p']['atividade']['2.0'])?$totalcc['p']['atividade']['2.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não realizou as atividades</td>
											<td style="font-size:x-small;"><?=(($totalcc['p']['atividade']['0'])?$totalcc['p']['atividade']['0']:'0') ?></td>
										</tr>
									</table>
									</td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Reprovado(s): <?=(($totalc['r'])?$totalc['r']:'0') ?></td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Participou integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['r']['participacao']['5,0'])?$totalcc['r']['participacao']['5,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['r']['participacao']['3,5'])?$totalcc['r']['participacao']['3,5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Participou insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['r']['participacao']['2,0'])?$totalcc['r']['participacao']['2,0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não Participou</td>
											<td style="font-size:x-small;"><?=(($totalcc['r']['participacao']['0.0'])?$totalcc['r']['participacao']['0.0']:'0') ?></td>
										</tr>
									</table>
									</td>
									<td>
									<table>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades integralmente</td>
											<td style="font-size:x-small;"><?=(($totalcc['r']['atividade']['5.0'])?$totalcc['r']['atividade']['5.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades suficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['r']['atividade']['3.5'])?$totalcc['r']['atividade']['3.5']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Realizou as atividades insuficientemente</td>
											<td style="font-size:x-small;"><?=(($totalcc['r']['atividade']['2.0'])?$totalcc['r']['atividade']['2.0']:'0') ?></td>
										</tr>
										<tr>
											<td style="font-size:x-small;">Não realizou as atividades</td>
											<td style="font-size:x-small;"><?=(($totalcc['r']['atividade']['0'])?$totalcc['r']['atividade']['0']:'0') ?></td>
										</tr>
									</table>
									</td>
								</tr>
							</table>
							
                            </td>
                        </tr>
                        <tr>
                        <td colspan="9">
                        
<? 

include_once APPRAIZ . "includes/library/simec/Grafico.php"; 

$grafico = new Grafico();

$sql = "select 
case when cavsituacao='e' then 'Evadido'
     when cavsituacao='f' then 'Faleceu'
     when cavsituacao='c' then 'Cursando'
     when cavsituacao='a' then 'Matriculado'
     when cavsituacao='b' then 'Desvinculado'
     when cavsituacao='d' then 'Trancado'
	 when cavsituacao='p' then 'Aprovado' 
	 when cavsituacao='r' then 'Reprovado' 
	 else 'Não identificada' end as descricao, 
m.mesdsc || ' / ' || fpbanoreferencia as categoria,
count(*) as valor 
from sisfor.cursistacurso cc 
inner join sisfor.cursistaavaliacoes ca on ca.cucid = cc.cucid 
inner join sisfor.folhapagamento f on f.fpbid = cc.fpbid
inner join public.meses m ON m.mescod::integer = f.fpbmesreferencia
where sifid=".$_GET['sifid']." 
group by ca.cavsituacao, m.mesdsc, f.fpbanoreferencia, f.fpbid
order by f.fpbid, descricao";

$grafico->setTitulo('Situação dos cursistas por mês')->setTipo(Grafico::K_TIPO_COLUNA)->setWidth('100%')->gerarGrafico($sql);

?>
                        
                        
                        
                        </td>
                        
                        </tr>
                            <?php } else { ?>
                        <tr bgcolor="" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#ffffcc';">
                            <td colspan="4" align="center"><span style="color: red;">Nenhum cursista vinculado</span></td>
                        </tr>
                    <?php } ?>
                </tbody>
            </table>
        </td>
    </tr>
    <tr>
    	<td class="SubTituloCentro"><input type="button" name="voltar" value="Voltar" onclick="window.location='sisfor.php?modulo=principal/mec/mec&acao=<?=$_REQUEST['acao'] ?>&aba=analise';"></td>
    </tr>
</table>