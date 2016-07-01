<?php
if($_FILES){
    if ( stripos($_FILES['arquivo']['name'], '.csv') != false ){

        $linhaZero = true;
        $row = 0;
        $planejamento = new PlanejamentoTatico();
        $handle = fopen ($_FILES['arquivo']['tmp_name'] ,"r");
        $error = array();
        while (($data = fgetcsv($handle, 0, ";")) !== FALSE) {
            if ( $linhaZero == true ){
                $linhaZero = false;
                continue;
            }
            $row++;

            $data = array_map('pg_escape_string', $data);
            $data = array_map('trim', $data);

            $prgcod          = $data[0];
            $acacod          = $data[1];
            $pltnome         = $data[2];
            $pltprioridade   = $data[3];
            $elaboracao      = explode(".", $data[4]);
            $pltcusteio      = MoedaToBd( $data[5] );
            $pltinvestimento = MoedaToBd( $data[6] );
            $uexsigla        = $data[8];
            $estuf           = $data[9];
            $mundescricao    = $data[10];
            $pltobservacao   = $data[11];

            $sql = "SELECT
                        COUNT(*)
                    FROM
                        monitora.programa
                    WHERE
                        prgstatus = 'A'
                        AND prgano = '{$_SESSION['exercicio']}'
                        AND sem_acento(prgcod) = sem_acento('{$prgcod}')";

            $existePrograma = $db->pegaUm($sql);

            $sql = "SELECT
                        COUNT(*)
                    FROM
                        monitora.acao
                    WHERE
                        acasnrap = false
                        AND acastatus = 'A'
                        AND prgano = '{$_SESSION['exercicio']}'
                        AND sem_acento(acacod) = sem_acento('{$acacod}')
                        AND sem_acento(prgcod) = sem_acento('{$prgcod}')";

            $existeAcao = $db->pegaUm($sql);

            $sql = "SELECT
                        DISTINCT
                        uexid
                    FROM
                        planointerno.unidadeexecutora
                    WHERE
                        uexstatus = 'A'
                        AND sem_acento(uexsigla) = sem_acento('{$uexsigla}')";

            $existeUnidade = $db->pegaUm($sql);
            $uexid = $existeUnidade;

            // Verificação da existências dos campos base (campos obrigatórios)
            if ( empty($existePrograma) || empty($existeAcao) || empty($existeUnidade) ){
                $error[] = $data;
                $row--;
                continue;
            }

            // Verificação de Unicidade do planejamentotatico
            $sql = "SELECT
                        pltid
                    FROM
                        sisplan.planejamentotatico
                    WHERE
                        pltstatus = 'A'
                        AND pltano = '{$_SESSION['exercicio']}'
                        AND uexid = '{$uexid}'
                        AND sem_acento(prgcod)  = sem_acento('{$prgcod}')
                        AND sem_acento(acacod)  = sem_acento('{$acacod}')
                        AND sem_acento(pltnome) = sem_acento('{$pltnome}')";

            $pltid = $db->pegaUm($sql);

            // Busca Elaboração
            $sql = "SELECT
                        stpid
                    FROM
                        sisplan.situacaoprocesso
                    WHERE
                        stpstatus = 'A'
                        AND sem_acento(stpdsc) ILIKE (sem_acento('%{$elaboracao[0]}%'))";

            $stpid = $db->pegaUm($sql);

            // Busca UF
            $sql = "SELECT
                        estuf
                    FROM
                        territorios.estado
                    WHERE
                        sem_acento(estuf) = sem_acento('{$estuf}')";

            $estuf = $db->pegaUm($sql);

            // Busca MUNICIPIO
            $sql = "SELECT
                        muncod
                    FROM
                        territorios.municipio
                    WHERE
                        sem_acento(estuf) = sem_acento('{$estuf}')
                        AND sem_acento(mundescricao) ILIKE (sem_acento('%{$mundescricao}%'))";

            $muncod = $db->pegaUm($sql);

            $arDado = array('pltano'          => $_SESSION['exercicio'],
                            'prgcod'          => $prgcod,
                            'acacod'          => $acacod,
                            'pltnome'         => $pltnome,
                            'pltprioridade'   => $pltprioridade,
                            'stpid'           => ($stpid ? $stpid : null),
                            'estuf'           => ($estuf ? $estuf : null),
                            'muncod'          => ($muncod ? $muncod : null),
                            'uexid'           => ($uexid ? $uexid : null),
                            'pltcusteio'      => ($pltcusteio ? $pltcusteio : 0),
                            'pltinvestimento' => ($pltinvestimento ? $pltinvestimento : 0),
                            'pltobservacao'   => $pltobservacao,
                            'usucpf'          => $_SESSION['usucpf']);
            if ( empty($pltid) ){
                $planejamento->popularDadosObjeto( $arDado )->salvar();
            }else{
                $planejamento->carregarPorId($pltid);
                $planejamento->popularDadosObjeto( $arDado )->salvar();
            }
            $planejamento->setDadosNull();
        }
        fclose ($handle);

        $db->commit();
    }else{
        die("<script>
                alert('Somente é permitida a importação por meio de arquivo de extensão (CSV)!');
                location.href = location.href;
             </script>");
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>
<form action="" method="post" name="formulario" enctype="multipart/form-data">
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Arquivo de Importação:
		    </td>
            <td>
                <input type="file" id="arquivo" name="arquivo" size="50" />
            </td>
		</tr>
		<?php if( $_FILES ) :?>
        <tr >
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                Relatório de importação:
            </td>
            <td>
                <div><b>Importados:</b> <?php echo ($row ? $row : 0);?></div>
                <div><b>Falhas de importação:</b> <?php echo count($error);?></div>
            </td>
        </tr>
		<?php endif; ?>
        <tr style="background-color: #cccccc">
            <td align="right" style="vertical-align:top; width:25%;">
                &nbsp;
            </td>
            <td colspan="">
                <input type="submit" name="btnImportar" value="Importar" onclick="enviar();"/>
                <input type="button" name="btnVoltar"   value="Cancelar" onclick="cancelar();"/>
            </td>
        </tr>
	</table>
</form>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

    function cancelar() {
		window.location.href = '?modulo=principal/pesquisa_planejamento&acao=C';
	}

</script>