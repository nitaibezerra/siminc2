<?php
$query = <<<DML
SELECT pgm.pgmdsc,
       prp.prptexto
  FROM altorc.perguntasrespostaspedido prp
    INNER JOIN altorc.perguntasmomento pgm USING(pgmid)
  WHERE prp.paoid = %d
DML;
$stmt = sprintf($query, $dadospao['paoid']);
if ($justificativas = $db->carregar($stmt)) {
    foreach ($justificativas as $just): ?>
<div>
    <p style="font-weight:bold"><?php echo $just['pgmdsc']; ?></p>
    <blockquote>
        <p style="font-size:12px"><?php echo $just['prptexto']; ?></p>
    </blockquote>
</div>
    <?php endforeach;
} else {?>
    <div class="alert alert-info col-md-4 col-md-offset-4 text-center">Nenhum registro encontrado</div>
<?php }
